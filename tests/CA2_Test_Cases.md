# CA2 Test Plan and Verification Record

| ID | Scenario | Expected result | Status |
|---|---|---|---|
| T-01 | Compile with C++11 and warnings | Build succeeds without warnings | Passed (local build) |
| T-02 | Add valid reading entry | Fixed-size record appended | Passed (scripted run) |
| T-03 | Restart application | Saved record is loaded | Passed (scripted run) |
| T-04 | Search existing Entry ID | Correct record displayed using seekg() | Passed (scripted run) |
| T-05 | Update pages read | Only pagesRead field overwritten using seekp() | Passed (scripted run) |
| T-06 | Restart after update | Updated pages-read value is retained | Passed (scripted run) |
| T-07 | Search missing Entry ID | Not-found message; file unchanged | Manual verification pending |
| T-08 | Duplicate Entry ID | Duplicate rejected | Manual verification pending |
| T-09 | Invalid page values | Values outside permitted range rejected | Manual verification pending |
| T-10 | Over-capacity text | Entry rejected without truncation | Manual verification pending |
| T-11 | Empty/missing data directory | Clear file-open error | Manual verification pending |
| T-12 | 100-entry capacity boundary | Additional record rejected safely | Manual verification pending |

## Scripted verification performed
- Compiled using `g++ -std=c++11 -Wall -Wextra -pedantic`.
- Added a sample record.
- Restarted the application and searched for the record.
- Updated pages read from 50 to 120.
- Restarted again and confirmed the saved value was 120/200 (60%).

Manual tests must be executed on the student's target computer and supported by genuine screenshots before submission. Do not change pending statuses without running those tests.
