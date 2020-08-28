CS 230 Project #4 Threads and Synchronization: Cure for Unknown Virus
Preston Yee

This project takes in a number of phone calls, and organizes these phone calls simultaneously, given a limited number of connections and operators, so that every patient on the phone call is able to receive the cure for a virus.

The program is structured so that it will:
- Take the number of phone calls via user input (and send an error message is user has not inputted a value)
- Initialize all of the semaphores necessary (operators, connected_locks, and caller_id)
- Create all of the phone call threads
- Make the phone calls (allow up to max of 8 lines for this project)
- Have 5 operators get the cure to those callers who are available
- If all of the lines are busy, the callers will have to wait for 1 second before calling again
- Join all of the threads together
- Destroy all of the semaphores at the end

Video Link: https://youtu.be/73cBXRXF64s