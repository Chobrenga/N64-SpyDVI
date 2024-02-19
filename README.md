# Meet the N64 SpyDVI

N64 DVI mod board using RP2040 and some DVI bit banging firmware from the [PicoDVI-N64](https://github.com/kbeckmann/PicoDVI-N64) project. Should be able to be installed as cut or no cut.

![](./Images/Board3D.png)

A QSB ribbon is planned for this project, but you can totally just solder the wires in accordance with Beckmann's diagram. I even left headers available for such a job.

The audio circuit is optional. 

The board only needs a 5V supply, there is a 3.3V regulator onboard to generate logic level. The RP2040 itself generates the 1.1V it needs for operaton.

Custom 3D printed bracket, braces, and facia are planned, but not yet available. The ones from Peter Bartmann's [n64adv2_pcb](https://github.com/borti4938/n64adv2_pcb) should work just fine.

# Fabrication Notes

This board has not been fabricated and tested as of yet.

R8 is intentionally missing on the BOM. It is not required.

This is intended for fabrication with JLC04161H-7628(Standard). Enable impedance control.

Select 0.15mm via option, 4-wire test is mandatory.

Hookup schematic for headers can be found on Konrad's [PicoDVI-N64](https://github.com/kbeckmann/PicoDVI-N64). The headers are labeled.

# Credit Given

Konrad Beckmann for initial inspiration and some initial firmware. Repo: [PicoDVI-N64](https://github.com/kbeckmann/PicoDVI-N64)

Wren6991 for sparking Konrad's project, and providing a functional DVI board schematic which my design is based on. [PicoDVI](https://github.com/Wren6991/PicoDVI)

# Board Layout

![](./Images/BRDFront.png)
![](./Images/BRDGround.png)
![](./Images/BRDPower.png)
![](./Images/BRDBack.png)

# Fitment

![](./Images/FusionTopView.png)
![](./Images/FusionBackView.png)
![](./Images/FusionFrontSectionView.png)
