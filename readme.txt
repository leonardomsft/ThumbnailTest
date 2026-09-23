Test 1 (works)

- Click File-Use Caption
  A window that refreshes once a second is displayed
 
- Click File-Thumbnail
  A window using DwmRegisterThumbail is shown below the "source" window

- Drag source window: You can click on caption or client area
  Thumbnail window is still visible, at the same position, mirroring source window

--------------------------------------------------------------------------------------

Test 2 (fails)

- Click File-No Caption
  A window that refreshes once a second is displayed
 
- Click File-Thumbnail
  A window using DwmRegisterThumbail is shown below the "source" window

- Drag source window: You can click on client area
  Thumbnail window is rendered (DWM?) at the wrong position.

Note that the issue does not depend on interactively moving the window
Instead of dragging source window, you can click File-Move Counter twice or more to reproduce the issue.
