from hexy.robot.pro import HexapodPro
from time import sleep

hexy = HexapodPro()


## Zero position
hexy.left_front.hip.drive(395)
hexy.left_front.knee.drive(340)
hexy.left_front.ankle.drive(420)

hexy.left_middle.hip.drive(321)
hexy.left_middle.knee.drive(450)
hexy.left_middle.ankle.drive(350)

hexy.left_back.hip.drive(360)
hexy.left_back.knee.drive(375)
hexy.left_back.ankle.drive(385)

hexy.right_front.hip.drive(315)
hexy.right_front.knee.drive(415)
hexy.right_front.ankle.drive(390)

hexy.right_middle.hip.drive(500)
hexy.right_middle.knee.drive(435)
hexy.right_middle.ankle.drive(390)

hexy.right_back.hip.drive(530)
hexy.right_back.knee.drive(355)
hexy.right_back.ankle.drive(390)

#hexy.left_front.hip.drive(395)
#hexy.left_front.knee.drive(200)
#hexy.left_front.ankle.drive(390)
#
#hexy.left_middle.hip.drive(321)
#hexy.left_middle.knee.drive(290)
#hexy.left_middle.ankle.drive(390)
#
#hexy.left_back.hip.drive(360)
#hexy.left_back.knee.drive(230)
#hexy.left_back.ankle.drive(615)
#
#hexy.right_front.hip.drive(315)
#hexy.right_front.knee.drive(320)
#hexy.right_front.ankle.drive(650)
#
#hexy.right_middle.hip.drive(445)
#hexy.right_middle.knee.drive(320)
#hexy.right_middle.ankle.drive(630)
#
#hexy.right_back.hip.drive(530)
#hexy.right_back.knee.drive(250)
#hexy.right_back.ankle.drive(630)

sleep(5)
hexy.off()
