# CA1 Test Cases — Book Reading Challenge Tracker

| ID | Test input / action | Expected result | Actual result |
|---|---|---|---|
| T01 | Add valid book: 240 total pages, 60 read | Record added; progress 25%; status In Progress | Pending execution |
| T02 | Add record with blank reader name | Blank value rejected and prompt repeated | Pending execution |
| T03 | Reuse an existing Entry ID | Duplicate rejected; no new record | Pending execution |
| T04 | Enter total pages as 0 | Rejected; valid range requested | Pending execution |
| T05 | Enter pages read greater than total pages | Rejected; valid range requested | Pending execution |
| T06 | Add book with 0 pages read | Status Not Started; progress 0% | Pending execution |
| T07 | Add book where pages read equals total pages | Status Completed; progress 100% | Pending execution |
| T08 | Search an existing Entry ID | Matching record displayed | Pending execution |
| T09 | Search a missing Entry ID | Clear not-found message | Pending execution |
| T10 | View all with no records | Empty-collection message | Pending execution |
| T11 | Add 100 records, then attempt another | 101st record rejected safely | Pending execution |
| T12 | Enter alphabetic text at numeric prompt | Invalid input rejected and prompt repeated | Pending execution |

Do not mark a test as passed until it has been run on the submitted source code. Save terminal screenshots in this directory as evidence.
