# Student Help Desk — Localhost Web Demo

This package contains the original Phase 2 C project plus a browser-based web demonstration.

## Run the website on localhost

1. Extract the ZIP.
2. Open PowerShell in the extracted `Student_Help_Desk_Localhost` folder.
3. Run:

   ```powershell
   py -m http.server 8000 --directory web
   ```

   If `py` is not recognized, try:

   ```powershell
   python -m http.server 8000 --directory web
   ```

4. Open Chrome and visit **http://localhost:8000**.
5. Keep the PowerShell window open while using the site. Press `Ctrl+C` in that window to stop the local server.

## Web demo features

- Submit student requests with student name, category, description and Low/Medium/High priority.
- Stable priority queue: High > Medium > Low; equal-priority requests retain arrival order (FIFO).
- Display pending requests and process the next request.
- Action history stack with Push (automatic on submit/process), Peek and Pop.
- Capacity limits of 100 pending requests and 100 history actions.
- Input validation for required fields.

## Important implementation note

The original `src/` and `include/` C files are preserved in this package and remain the C implementation for the Data Structures project. The localhost web demo is a separate browser-side demonstration of the same queue and stack behavior; it does not execute or compile the C binary from the browser.

The web demo keeps data in memory only. Refreshing or closing the page resets the demonstration data.

## Original C program

See the original `README.md` for Windows/Linux build and automated test instructions.
