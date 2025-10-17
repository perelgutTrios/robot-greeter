const express = require('express');
const http = require('http');
const socketIo = require('socket.io');
const sqlite3 = require('sqlite3').verbose();
const cors = require('cors');
const path = require('path');
const mqtt = require('mqtt');
const multer = require('multer');
const axios = require('axios');
const fs = require('fs').promises;

const app = express();
const server = http.createServer(app);
const io = socketIo(server, {
  cors: {
    origin: "*",
    methods: ["GET", "POST"]
  }
});

// Database setup
const db = new sqlite3.Database('./greetings.db');
db.run(`CREATE TABLE IF NOT EXISTS greetings (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  name TEXT NOT NULL,
  greeting TEXT NOT NULL,
  timestamp DATETIME DEFAULT CURRENT_TIMESTAMP
)`);

db.run(`CREATE TABLE IF NOT EXISTS visitors (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  name TEXT,
  image_path TEXT NOT NULL,
  face_descriptors TEXT,
  last_seen DATETIME DEFAULT CURRENT_TIMESTAMP,
  visit_count INTEGER DEFAULT 1
)`);

// MQTT Client setup
const mqttClient = mqtt.connect('mqtt://localhost:1883');
mqttClient.on('connect', () => {
  console.log('Connected to MQTT broker');
  mqttClient.subscribe('robot/+/status');
  mqttClient.subscribe('robot/+/visitor_detected');
});

// File upload configuration
const storage = multer.diskStorage({
  destination: (req, file, cb) => {
    cb(null, 'uploads/');
  },
  filename: (req, file, cb) => {
    const timestamp = Date.now();
    cb(null, `visitor_${timestamp}_${file.originalname}`);
  }
});

const upload = multer({ 
  storage: storage,
  limits: { fileSize: 10 * 1024 * 1024 }, // 10MB limit
  fileFilter: (req, file, cb) => {
    if (file.mimetype.startsWith('image/')) {
      cb(null, true);
    } else {
      cb(new Error('Only image files are allowed'));
    }
  }
});

// Middleware
app.use(cors());
app.use(express.json());
app.use(express.static('public'));

// Greeting templates
const greetingTemplates = [
  "Hello {name}! I'm your friendly robot assistant.",
  "Greetings {name}! How can I help you today?",
  "Welcome {name}! Ready to explore together?",
  "Hi there {name}! Your robot companion is online and ready.",
  "Salutations {name}! Let's make today awesome together!"
];

// API Routes
app.post('/api/greet', (req, res) => {
  const { name } = req.body;
  
  if (!name || name.trim().length === 0) {
    return res.status(400).json({ error: 'Name is required' });
  }

  const sanitizedName = name.trim().replace(/[<>&"']/g, '').substring(0, 50);
  const template = greetingTemplates[Math.floor(Math.random() * greetingTemplates.length)];
  const greeting = template.replace('{name}', sanitizedName);

  // Save to database
  const localTimestamp = new Date().toISOString();
  db.run('INSERT INTO greetings (name, greeting, timestamp) VALUES (?, ?, ?)', [sanitizedName, greeting, localTimestamp], function(err) {
    if (err) {
      console.error('Database error:', err);
      return res.status(500).json({ error: 'Database error' });
    }

    const response = {
      id: this.lastID,
      name: sanitizedName,
      greeting: greeting,
      timestamp: localTimestamp
    };

    // Broadcast to all connected clients
    io.emit('newGreeting', response);
    
    res.json(response);
  });
});

app.get('/api/greetings', (req, res) => {
  db.all('SELECT * FROM greetings ORDER BY timestamp DESC LIMIT 10', (err, rows) => {
    if (err) {
      return res.status(500).json({ error: 'Database error' });
    }
    res.json(rows);
  });
});

app.get('/api/stats', (req, res) => {
  db.get('SELECT COUNT(*) as total FROM greetings', (err, row) => {
    if (err) {
      return res.status(500).json({ error: 'Database error' });
    }
    res.json({ totalGreetings: row.total });
  });
});

// Visitor recognition endpoints
app.post('/api/visitors/upload', upload.single('image'), async (req, res) => {
  if (!req.file) {
    return res.status(400).json({ error: 'No image file provided' });
  }

  try {
    const imagePath = req.file.path;
    const { robotId = 'unknown' } = req.body;

    // Simulate face detection/recognition (replace with actual face-api.js implementation)
    const faceData = await processFaceImage(imagePath);
    
    if (!faceData.detected) {
      await fs.unlink(imagePath); // Clean up if no face detected
      return res.status(400).json({ error: 'No face detected in image' });
    }

    // Check against remote database
    const matchResult = await checkRemoteDatabase(faceData.descriptors);
    
    let visitor;
    if (matchResult.match) {
      // Known visitor - update last seen
      visitor = await updateVisitorRecord(matchResult.visitorId, imagePath);
      
      // Send personalized greeting via MQTT
      mqttClient.publish(`robot/${robotId}/greeting`, JSON.stringify({
        type: 'returning_visitor',
        name: visitor.name,
        visitCount: visitor.visit_count,
        lastSeen: visitor.last_seen
      }));
    } else {
      // New visitor - create record
      visitor = await createVisitorRecord(imagePath, faceData.descriptors);
      
      // Send new visitor greeting via MQTT
      mqttClient.publish(`robot/${robotId}/greeting`, JSON.stringify({
        type: 'new_visitor',
        visitorId: visitor.id,
        timestamp: new Date().toISOString()
      }));
    }

    // Broadcast to web clients
    io.emit('visitorDetected', {
      robotId,
      visitor: visitor,
      isReturning: matchResult.match
    });

    res.json({
      success: true,
      visitor: visitor,
      isReturning: matchResult.match,
      confidence: matchResult.confidence || 0
    });

  } catch (error) {
    console.error('Visitor processing error:', error);
    res.status(500).json({ error: 'Failed to process visitor image' });
  }
});

app.get('/api/visitors', (req, res) => {
  db.all('SELECT id, name, last_seen, visit_count FROM visitors ORDER BY last_seen DESC LIMIT 20', (err, rows) => {
    if (err) {
      return res.status(500).json({ error: 'Database error' });
    }
    res.json(rows);
  });
});

app.post('/api/visitors/:id/identify', (req, res) => {
  const { id } = req.params;
  const { name } = req.body;
  
  if (!name || name.trim().length === 0) {
    return res.status(400).json({ error: 'Name is required' });
  }

  const sanitizedName = name.trim().replace(/[<>&"']/g, '').substring(0, 100);
  
  db.run('UPDATE visitors SET name = ? WHERE id = ?', [sanitizedName, id], function(err) {
    if (err) {
      return res.status(500).json({ error: 'Database error' });
    }
    
    if (this.changes === 0) {
      return res.status(404).json({ error: 'Visitor not found' });
    }
    
    // Broadcast update
    io.emit('visitorIdentified', { id: parseInt(id), name: sanitizedName });
    
    res.json({ success: true, name: sanitizedName });
  });
});

// Helper functions for face processing
async function processFaceImage(imagePath) {
  // Placeholder for face-api.js implementation
  // In real implementation, load the image and extract face descriptors
  return {
    detected: true,
    descriptors: `mock_descriptor_${Date.now()}`, // Replace with actual face descriptors
    confidence: 0.95
  };
}

async function checkRemoteDatabase(descriptors) {
  try {
    // Simulate remote database check
    // Replace with actual API call to your remote face database
    const response = await axios.post('https://api.example.com/faces/match', {
      descriptors: descriptors,
      threshold: 0.6
    }, {
      timeout: 5000,
      headers: {
        'Authorization': 'Bearer YOUR_API_KEY',
        'Content-Type': 'application/json'
      }
    });
    
    return response.data;
  } catch (error) {
    console.log('Remote database unavailable, treating as new visitor');
    return { match: false, confidence: 0 };
  }
}

async function updateVisitorRecord(visitorId, imagePath) {
  return new Promise((resolve, reject) => {
    const localTimestamp = new Date().toISOString();
    db.run('UPDATE visitors SET last_seen = ?, visit_count = visit_count + 1, image_path = ? WHERE id = ?', 
      [localTimestamp, imagePath, visitorId], function(err) {
      if (err) return reject(err);
      
      db.get('SELECT * FROM visitors WHERE id = ?', [visitorId], (err, row) => {
        if (err) return reject(err);
        resolve(row);
      });
    });
  });
}

async function createVisitorRecord(imagePath, descriptors) {
  return new Promise((resolve, reject) => {
    const localTimestamp = new Date().toISOString();
    db.run('INSERT INTO visitors (image_path, face_descriptors, last_seen) VALUES (?, ?, ?)', 
      [imagePath, descriptors, localTimestamp], function(err) {
      if (err) return reject(err);
      
      db.get('SELECT * FROM visitors WHERE id = ?', [this.lastID], (err, row) => {
        if (err) return reject(err);
        resolve(row);
      });
    });
  });
}

// WebSocket connections
io.on('connection', (socket) => {
  console.log('Client connected:', socket.id);
  
  socket.emit('robotStatus', { 
    status: 'online', 
    message: 'Robot Greeter is ready to assist!' 
  });

  // Handle manual visitor identification from web interface
  socket.on('identifyVisitor', (data) => {
    const { visitorId, name } = data;
    db.run('UPDATE visitors SET name = ? WHERE id = ?', [name, visitorId], function(err) {
      if (!err && this.changes > 0) {
        socket.broadcast.emit('visitorIdentified', { id: visitorId, name });
      }
    });
  });

  socket.on('disconnect', () => {
    console.log('Client disconnected:', socket.id);
  });
});

// MQTT message handlers
mqttClient.on('message', (topic, message) => {
  console.log(`MQTT received: ${topic} - ${message.toString()}`);
  
  try {
    const data = JSON.parse(message.toString());
    
    if (topic.endsWith('/status')) {
      // Robot status update
      io.emit('robotStatusUpdate', {
        robotId: topic.split('/')[1],
        status: data
      });
    } else if (topic.endsWith('/visitor_detected')) {
      // Hardware detected a visitor
      io.emit('hardwareVisitorDetection', {
        robotId: topic.split('/')[1],
        detection: data
      });
    }
  } catch (error) {
    console.error('MQTT message parsing error:', error);
  }
});

// Serve the main page
app.get('/', (req, res) => {
  res.sendFile(path.join(__dirname, 'public', 'index.html'));
});

const PORT = process.env.PORT || 3000;
server.listen(PORT, () => {
  console.log(`Robot Greeter server running on port ${PORT}`);
  console.log(`Visit: http://localhost:${PORT}`);
});