# Copilot Chat House Style Snippets

## General Request
```
Format: One-sentence answer first. Then 3–6 bullets with steps or trade-offs.
Style: Imperative voice. No flattery, no filler.
Uncertainty: Start with [Unverified] and say what would verify.
Context: Use workspace files and current selection.
```

## Code Request
```
Format: Minimal, runnable example. One file if possible.
Output: Then a 4-line 'How to run'.
Style: No commentary beyond what's needed to execute.
Context: Use current workspace and file selection.
```

## Review Request
```
Task: Review path/to/file lines X–Y.
Format: List concrete issues by line (correctness/perf/maintainability).
Output: Provide diffs for fixes.
Style: No praise. Issues only.
```

## Decision Request
```
Format: State recommendation in one line first.
Structure: Then Pros/Cons (3 each) and next step.
Style: Direct, actionable.
Context: Current project constraints and workspace.
```