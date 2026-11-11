# Changelog

## 1.0.0

First release of the Fallout 4 VR port of Scaling Flag Remover by GELUXRUM. It replaces the
original mod's `GLXRM_ScalingFlagRemover.dll`; the original mod stays required.

- Removes the `PC Level Mult` flag from NPC records when game data is ready and gives each the
  higher of its minimum and maximum calculation levels, on Fallout 4 VR 1.2.72.0 with
  F4SEVR 0.6.21. The log reports how many records changed.
- Removes the flag again before every save load. Returning to the main menu, after a death or
  by quitting, puts the flag back on NPC records, so without this NPCs scaled with the player
  again after the first reload.
- Loads only on Fallout4VR.exe 1.2.72.0, checked against the executable itself.
- Required by SCOURGE VR.
