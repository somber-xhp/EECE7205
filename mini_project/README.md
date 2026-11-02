# Airport Code Lookup Service with a Binary Search Tree

C++17 mini project for the supplied OpenFlights airport data. The two domain
classes are `Airport` (record) and `AirportBST` (private root, pointer-linked
nodes). No library container implements the tree. Standard stacks/queues are
used only for iterative traversals; `std::set` is used only as a test oracle.

## Build and run

Run from the repository root:

```bash
g++ -std=c++17 -O2 src/*.cpp -o airports
./airports
```

For stricter compilation and the complete test/experiment run:

```bash
./tests/run_tests.sh
# Or separately:
./airports --test
./airports --experiment
./airports < docs/demo_commands.txt
```

## Commands

| Command | Behavior |
|---|---|
| `load data/airports.dat` | Parse and insert the supplied master data in file order |
| `load data/airports.csv` | Equivalent CSV input; skips the header and empty keys |
| `find BOS` | Print all compared codes, count, and full airport record |
| `minmax` | Print the minimum and maximum keys |
| `stats` | Count, edge-based height and average node depth |
| `top` | Breadth-first codes for depths 0, 1, 2 |
| `list` | All records in strictly increasing IATA-code order |
| `remove AEY` | Delete using the appropriate case, then validate |
| `add QQQ "Somber Xie Test Airport" "Troy"` | Add a fictional record, then validate |
| `save docs/saved.txt` | Write one preorder code per line |
| `load docs/saved.txt` | Rebuild from saved codes and the loaded catalog |
| `validate` | Check strict inherited key bounds |
| `range BOS BWI` | Inclusive sorted range, plus examined-node count |
| `rebalance` | Rebuild from inorder records using the middle rule |
| `quit` | Release all nodes and exit |

Keys are case-sensitive; use the codes exactly as supplied. `!` and `~` are
intentional lower/upper boundary test strings, not airport codes. Height is
measured in edges, root depth is 0, and empty-tree height is -1.

Saved `.txt` files contain **codes only**, as required. Load the master DAT/CSV
first in a fresh session so saved codes can be resolved to airport records.
Fictional records remain in the current session's catalog after removal;
across sessions, re-add fictional records before loading a saved file that
references them. Unknown/duplicate saved codes are rejected without replacing
the existing tree. Use `.txt` for saved-code files and `.dat`/`.csv` for data.
The `add` command supplies test defaults for country, ICAO and coordinates;
real records are imported with all original fields.

## Data checks and important input detail

- 7,698 airport data rows; 1,626 missing IATA values skipped.
- 6,072 inserted keys; zero duplicates; 235 countries; 1,251 US airports.
- Minimum `AAA`; maximum `ZZV`; initial root `GKA`.
- Keep `DU9` (Dunnville Airport). It is present in the supplied data even though
  it contains a digit. Filtering to letters alone incorrectly produces 6,071.
- Quoted commas and doubled quotes are handled by a character-by-character
  parser. The uploaded DAT and CSV versions have matching code/name records.

## Sample session (measured output)

```text
> load data/airports.dat
Loaded 6072 airports (1626 lines skipped: no IATA code). Duplicates: 0
Count: 6072 Height: 177 Average depth: 18.323287
validate: OK
> find BOS
Path: GKA -> AEY -> EGS -> BUJ -> BJA -> BSK -> BOY -> BNI -> BOH -> BOO -> BOU -> BOS (comparisons: 12)
> range BOS BWI
113 airports, 127 nodes visited
> remove AEY
Deleted AEY (D3: two children; successor AFA)
validate: OK
```

See `docs/demo_output.txt` for the actual unabridged terminal session, including
every airport printed by the range query.

## Task 6: measured shape experiment

All rows use the same 10,000 successful queries sampled with replacement by
`mt19937`, seed 2560. The random insertion permutation also uses seed 2560.
Timing uses `steady_clock` and excludes construction, validation, output and
path collection. It includes the comparison counter and result checksum.
Numbers below are one real run, not expected constants; timings vary by host.

| Build order | Height | Average depth | Time for 10,000 searches (ms) | Comparisons |
|---|---:|---:|---:|---:|
| File order | 177 | 18.323287 | 2.044554 | 196675 |
| Sorted order | 6071 | 3035.500000 | 316.121711 | 30354864 |
| Balanced | 12 | 10.653162 | 1.657192 | 116441 |
| Random shuffle | 31 | 14.532773 | 1.785645 | 155429 |

The file is ordered by airport IDs in geographic/country blocks rather than a
random code permutation. For example, the early Canadian block begins YAM,
YAY, YAZ, YBB, YBC, YBG, YBK, YBL, YBR, YCB, ... . Long stretches of similar,
nearly increasing Y-codes create deep paths; the deepest key YZR has 178
comparisons, hence depth 177. Sorted insertion is entirely increasing, making
a chain of 6072 nodes and average depth (6072-1)/2 = 3035.5. The middle-rule
build splits each subproblem almost equally, yielding height 12 and average
depth 10.653162. Shuffling removes this systematic order bias. Our particular
seed gives height 31; the handout's 25-29 is an approximate typical range,
not a bound on every random permutation. All four checksum values agree.

Single insert/search/delete costs O(h+1) (usually written O(h)); worst case
O(n), and O(log n) for a balanced tree. The ordinary tree does not maintain
balance after later mutations. Range traversal costs O(h+k) for k results,
with O(h) traversal stack plus O(k) returned records. Full traversals,
validation and metrics cost O(n). Middle-rule construction from an already
sorted vector costs O(n), with O(log n) recursion depth. Sorting first costs
O(n log n). Repeated sorted insertion costs O(n^2). Save is O(n); saved-code
reload uses catalog sorting/lookup plus ordinary insertion, worst O(n^2).
Iterative insertion, search, traversal, validation and cleanup avoid recursion
on the 6071-edge chain.

## Verification and evidence

- `docs/pseudocode.md`: pseudocode before each task, including all operations.
- `docs/test_results.txt`: actual complete output for every required case.
- `docs/experiment.csv` and `.txt`: machine-readable and readable measurements.
- `docs/preorder.txt`: all 6072 codes in saved preorder.
- `docs/inorder_codes.txt`: all 6072 codes in sorted order.
- `docs/sanitizer_results.txt`: same tests under AddressSanitizer and UBSan.
- `docs/demo_output.txt`: complete captured CLI demonstration transcript.

Mutation tests validate after every insertion/deletion. They cover all five
real-data deletion categories, a fictional QQQ add/find/remove, full record
preservation after deletion, preorder/full-level-order/height equality after
reload, and 1000 randomized mutations checked against a test-only std::set.
Synthetic tests cover root with zero/one/two children, missing/empty deletion,
both successor link cases and successor right-child promotion, duplicate
rejection, and CSV quoting. Range output is cross-checked against an inorder
filter, including singleton, empty, reversed and full-domain boundaries.

Strict build passed with `-Wall -Wextra -Werror -pedantic`. AddressSanitizer
and UBSan tests passed with leak detection disabled because this sandbox
blocks LeakSanitizer's process inspection. No separate leak-check result is
claimed. The program owns and deletes all nodes via its destructor.

## Repository location

This project is stored in `mini_project/` in the EECE7205 repository:
https://github.com/somber-xhp/EECE7205

From the repository root, run `cd mini_project` before the build and run
commands above. All project-relative paths are based on this folder.

## Data credit

Airport data: **OpenFlights.org**, supplied course copy of `airports.dat`
and its CSV equivalent. Source: https://openflights.org/data.php ; upstream:
https://github.com/jpatokal/openflights . The database is published under the
Open Database License (ODbL); see the source for license terms. Do not infer
an MIT or other software license for the data. No software license is chosen
on behalf of the student.
