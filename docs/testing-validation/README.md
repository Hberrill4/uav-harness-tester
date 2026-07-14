## Testing & Validation
## Possible errors
When a wire loom less than 64 wires is entered it detects the lack of wire as an open circuit and would siplay an error message and be hard to discern from the actual errors.
# Resolution: Have those empty wire spaces be treated as suc when the golden sample is submitted so that when a test harness is connected those wire positions are treated as not part of the test and input is 0.
When the golden sample is submitted, if any wire is loose or not properly detected the microcontroller will detect it as not existin and that will be part of the stored profile for it. This would mean this particular wire would not be properly tested and may display faulty results.
# Resolution:


*Content to be added.*
