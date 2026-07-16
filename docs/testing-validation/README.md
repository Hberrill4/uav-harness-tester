# Testing & Validation
## Possible errors in thought process
#### 13/07/2026
When a wire loom less than 64 wires is entered it detects the lack of wire connection as an open circuit and would display an error message and be hard to discern from the actual errors.
**Resolution:** Have those empty wire spaces be treated as such when the golden sample is submitted so that theyre saved as not existn meaning when a test harness is connected those wire positions are treated as not part of the test and input is 0 and no error message is displayed.
#### 14/07/2026
When the golden sample is submitted, if any wire is loose or not properly detected the microcontroller will detect it as not existin and that will be part of the stored profile for it. This would mean this particular wire would not be properly tested and may display faulty results.
##### Resolution:


#### 16/07/2026
Circumstance where more than one error is presented as there may be both a short and wire mismatch. how to distinguish this and how to report it?



*Content to be added.*
