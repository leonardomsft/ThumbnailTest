Repro Steps: 


Test 1 (working)

- Click File -> Create window with Caption
  A window with a title bar is created
 
- Click File -> Create Thumbnail
  A thumbnail window is created below the "source" window

- Drag source window (using the tittle bar or client area)
  The thumbnail window is still visible, at the same position, mirroring source window

--------------------------------------------------------------------------------------

Test 2 (non-working)

- Click File -> Create window with No Caption
  A borderless window is created
 
- Click File -> Create Thumbnail
  A thumbnail window is created below the "source" window

- Drag source window: (using the tittle bar or client area)
  Thumbnail window is rendered at the wrong position.
