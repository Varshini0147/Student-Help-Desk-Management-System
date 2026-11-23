# Test cases

Run each test manually in the program and record the actual result after execution.

| Test | Steps | Expected result |
|---|---|---|
| Priority ordering | Add Low, then High; display | High appears before Low |
| FIFO tie-break | Add two High requests in order; display/process | First High request is processed first |
| Empty queue | Select Process Next before adding requests | Safe “No pending requests” message |
| Stack PUSH | Submit a request | Submission action appears in history |
| Stack POP | Submit two requests, then POP | Most recent action is returned first |
| Empty stack | POP before any action is recorded | Safe underflow message |
| Invalid menu | Enter 9 | Invalid choice message and menu repeats |
| Invalid priority | Enter 5 while submitting | Prompt repeats until 1, 2 or 3 |
