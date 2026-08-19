# Design Enclosure

# Harness Tester Enclosure — OnShape Build Guide

A note before you start: OnShape's toolbar occasionally shifts icon positions between updates. Everything below reflects the standard, current toolset — if an icon isn't exactly where described, hover over the toolbar for tooltips, or use the search (magnifying glass icon, top toolbar) and type the tool name.

Also worth remembering: on OnShape's free tier, documents are public by default. Since this is Greenjets IP, consider whether that's acceptable before you build much further into it.

---

## Before You Start: Measurements Checklist

Fill in the placeholders before you get deep into modeling — you'll need most of these by Section 3.

| Item | Value | Status |
|---|---|---|
| Main PCB length | `[PCB_LENGTH]` mm | **Measure from your KiCad board outline** |
| Main PCB width | `[PCB_WIDTH]` mm | **Measure from your KiCad board outline** |
| Main PCB mounting hole positions (X,Y from corner) | `[PCB_HOLE_1]`, `[PCB_HOLE_2]`, etc. | **Pull from KiCad** |
| Tallest component stack height (ESP32-S3 DevKit + header socket) | `[STACK_HEIGHT]` mm | **Measure physical stack** |
| Wall thickness | 2.5 mm | Suggested default — adjust if you want |
| Standoff height (PCB to floor) | 5.5 mm | Suggested default |
| D-sub flange hole spacing | 63.5 mm | Confirmed (HARTING) |
| D-sub flange hole diameter | 3.1 mm | Confirmed (HARTING) |
| D-sub shell cutout size | Use imported STEP bounding box | Don't hand-measure — see Section 5 |
| Screen mounting hole spacing | 57.2 × 74.9 mm | Confirmed (Adafruit) |
| Screen PCB thickness | 1.6 mm (bare) / 7.9 mm (full stack) | Confirmed (Adafruit/RS) |
| Screen active area | `[SCREEN_ACTIVE_W]` × `[SCREEN_ACTIVE_H]` mm | **Measure your physical unit** |
| Button cutout diameter | 30 mm | Confirmed (Sanwa) |
| Wall thickness at button | 2.0–3.7 mm | Confirmed — must match your global wall thickness or get a local pad |

---

## 0. OnShape Setup

1. Click **Create new document** from your dashboard, name it something like `Harness Tester Enclosure`.
2. You land in a default **Part Studio** tab (bottom left). Rename this tab to `Enclosure` by double-clicking its label.
3. Confirm units are millimeters: click the document name (top left) → **Edit** → check **Length unit = mm**. If you're not there already, switch it now — mixing units mid-project is a common source of errors.

---

## 1. Define Your Variables

Parametrize the whole model now so changing one number (say, wall thickness) updates everything downstream.

1. In the Part Studio toolbar, click the **fx (Variables)** icon.
2. Create a new **Variable Studio** if prompted, or add directly to the Part Studio's variable table.
3. Add these variables one at a time (name, value):
   - `#wallThk = 2.5 mm`
   - `#standoffH = 5.5 mm`
   - `#pcbL = [PCB_LENGTH] mm`
   - `#pcbW = [PCB_WIDTH] mm`
   - `#dsubHoleSpace = 63.5 mm`
   - `#dsubHoleDia = 3.1 mm`
   - `#buttonDia = 30 mm`
   - `#screenHoleX = 57.2 mm`
   - `#screenHoleY = 74.9 mm`
4. Close the Variable Studio. From now on, when a sketch or feature asks for a dimension, type `#variableName` instead of a raw number — OnShape will link it live.

---

## 2. Import Your STEP Files

1. In the Part Studio, right-click in the empty canvas (or use **Insert** in the top menu) → **Insert** → choose **Import**.
2. Select your HARTING D-sub STEP file. It'll appear as a new solid body in the Part Studio feature list, positioned wherever its native origin was.
3. Repeat for the button STEP file.
4. Rename both in the feature list (double-click) to something clear: `DSUB_CONNECTOR` and `ARCADE_BUTTON`. You'll reference these names constantly from here on.
5. Leave both roughly where they landed for now — you'll reposition them precisely in Sections 5 and 6.

---

## 3. Build the Base Tray Shell

1. Start a new **Sketch** on the **Top** plane.
2. Draw a rectangle centered on the origin. Set its dimensions using your variables: width = `#pcbW + 2*wallThk + 20mm` (the +20mm is clearance margin — adjust once you know your real connector footprint from Section 5), length similarly generous to fit the D-sub couplers, screen, and button per the panel layout you already worked out.
3. Close the sketch.
4. Click **Extrude**, select the rectangle, extrude **Blind** upward to your target enclosure height (start with a placeholder like 60mm — you'll refine this once component stack heights are confirmed). This solid is your "shell blank."
5. Click **Shell**. Select the **top face** of the blank as the face to remove, set thickness to `#wallThk`. This hollows the block into an open-topped tray with walls of your set thickness.

You now have a bare tray. Everything else — bosses, cutouts, lid — builds on this.

---

## 4. Add PCB Standoffs

1. New **Sketch** on the tray's interior floor.
2. At each of your `[PCB_HOLE]` placeholder positions, draw a circle for the standoff boss (outer diameter ~6mm is typical for an M3 self-tap boss; adjust once you pick screw type) and a smaller concentric circle for the pilot hole (for M3 self-tapping, pilot hole ≈ 2.5mm; for a heat-set insert, size to your insert's spec sheet — usually 3.5–4mm for an M3 insert).
3. **Extrude** the boss circles upward from the floor by `#standoffH`, merged with the tray body (Boolean **Add** — OnShape usually merges automatically if the geometry touches, but check the operation type in the Extrude dialog).
4. **Extrude Cut** the pilot holes down through the boss height (doesn't need to go through the floor unless you want a through-hole for assembly-side access).

---

## 5. Position and Cut the D-Sub Coupler Openings

This is where having the real STEP file pays off — don't use estimated dimensions, use the actual imported geometry.

1. Select the `DSUB_CONNECTOR` body. Click **Transform** (Move/Copy tool).
2. Use **Translate** to move it into position against the interior face of whichever wall will hold the couplers. Position it so the connector's mating face sits flush with (or just proud of) the exterior wall surface — you're essentially placing it exactly where it'll sit once mounted.
3. Before finalizing position, measure the imported body's actual bounding box (click on it, check the dimension readout in the sidebar, or use **Measure** tool) — this replaces my earlier 54×16mm estimate with your real number.
4. Once positioned, click **Transform** → **Copy**, and create a second instance for the lower coupler, offset vertically by your chosen spacing (15–20mm gap between flange edges, per the earlier layout).
5. Select the tray body as the **target**, and both `DSUB_CONNECTOR` copies as **tool bodies**. Click **Boolean**, set operation to **Subtract**. This cuts both connector-shaped openings directly from the real geometry — a perfect fit, no manual dimensioning.
6. New **Sketch** on the exterior wall face, at each cutout, add 2 circles per connector at `#dsubHoleSpace` apart, diameter `#dsubHoleDia`. Extrude cut through the wall — these are your flange screw holes.
7. On the interior side, sketch and extrude a shallow rectangular pocket matching the connector's flange footprint, deep enough (1–2mm) that the flange sits recessed and flush when screwed down — this is what actually takes the mechanical load, not the wall material around the cutout.

---

## 6. Position and Cut the Button Opening

1. Select `ARCADE_BUTTON`, use **Transform** to move it to its intended panel location (per your front-panel layout).
2. Same approach as the connector: **Boolean Subtract** the button body from the tray to get a geometrically accurate snap-fit opening, rather than hand-sketching a 30mm circle. This matters more than it seems — the Sanwa snap clips have specific internal geometry that's easy to get slightly wrong by hand.
3. **Check wall thickness at this location** — Sanwa spec is 2.0–3.7mm. If your global `#wallThk` (2.5mm) applies uniformly here, you're already fine. If this wall section got thickened by another feature, add a local pocket to bring it back into spec.

---

## 7. Model the Screen Window (Manual)

No STEP file exists for this one, so it's built from your measurements.

1. New **Sketch** on the exterior face of the screen wall.
2. Draw a rectangle sized to `[SCREEN_ACTIVE_W]` − 2–3mm × `[SCREEN_ACTIVE_H]` − 2–3mm (this undersizing creates the bezel lip that overlaps and hides the display's black border).
3. **Extrude Cut** through the full wall thickness — this is the visible window opening.
4. On the **interior** face, sketch a second, larger rectangle sized to the full glass/module outline (a few mm larger than active area on each side) and extrude cut only partway through the wall (about half of `#wallThk`) — this creates the recessed step/lip that the display module sits against from behind, so it can't push through the front.
5. Add 4 standoff bosses (same method as Section 4) at `#screenHoleX` × `#screenHoleY` spacing, set standoff height so the display's front glass lands flush with, or very slightly behind, the outer wall surface — use the 7.9mm full-stack height and 1.6mm PCB thickness to work out the correct standoff height by subtraction.

---

## 8. Cable Routing Features

1. Sketch small retention tabs or a shallow channel (2–3mm wide, 1–2mm deep) along the interior floor, tracing a smooth-radius path from each D-sub coupler's rear and the screen's ribbon location back toward where the main PCB will sit. Extrude these as raised ribs or cut channels, whichever your routing plan calls for.
2. At the button location, if the button will be external to the enclosure (wired, not enclosure-mounted), sketch a small two-piece gland: a semi-circular channel matching your wire's diameter, split across the parting line between two printed halves that clamp together — model this as a separate small part if you want it removable, or integrate it directly into the wall if permanent.

---

## 9. Design the Lid

1. Create a **new Sketch** on the Top plane (or derive from the tray's top face outline).
2. Extrude a shallow lid shape — roughly `#wallThk + 2mm` deep — matching the tray's outer perimeter.
3. Add a **tongue**: extrude a thin rib (1.5–2mm tall, 1–1.5mm wide) around the inside perimeter of the lid, sized to slip into a matching **groove** cut into the top edge of the tray wall (cut this groove back in the Section 3 tray body via a new Extrude Cut around its top perimeter). This is what keeps the two halves aligned and keeps dust out.

---

## 10. Lid Fastening Bosses

1. Add 4 (or more, depending on size) screw bosses at the corners of both the tray and lid, aligned so they meet when assembled.
2. Tray-side bosses: solid, with a pilot hole sized for your chosen fastener (M3 self-tap pilot ≈ 2.5mm, or heat-set insert bore per your insert's spec).
3. Lid-side bosses: through-holes only, sized as clearance (3.2mm for M3), so the screw passes through the lid and threads into the tray boss beneath it.

---

## 11. Check Fit and Interferences

1. Before exporting, use **Tools** → **Interference Detection** (or similar, under the Tools menu) to check that the connector cutouts, standoffs, and lid don't overlap unintentionally.
2. Manually verify: does the tallest component stack (Section 0 checklist) clear the lid interior? Does the screen standoff height actually land the glass where you intended? Does the button's local wall thickness sit inside 2.0–3.7mm?

---

## 12. Export for Printing

1. Right-click each final body (tray, lid, and any separate small parts like the wire gland) in the feature list or in the **Part** tab.
2. Select **Export**.
3. Choose format **STL** (or STEP if you want to keep it parametric for a slicer that supports it), set resolution to a fine tolerance for accurate curves, and download.
4. Import each STL into your slicer, check orientation per the earlier print-orientation guidance (X-Y plane holds the critical dimensions — screw spacing, cutout shapes), and slice.

---

## Final Considerations Before You Print

- **Print a test fit of just the tray first** (as covered earlier) — check the D-sub and button cutouts against your real connectors before committing to a full print with the lid and all details.
- **PETG over PLA** if this will ever sit near anything warm on the bench.
- **Heat-set inserts** are worth the extra step over self-tapping screws if you'll be opening this enclosure repeatedly for debugging — self-tap threads in FDM plastic degrade after 5-10 insertions.
