(define (problem gripper-1-2-6)
(:domain gripper-strips)
(:objects robot1 - robot
rgripper1 lgripper1 - gripper
room1 room2 - room
ball1 ball2 ball3 ball4 ball5 ball6 - object)
(:init
(at-robby robot1 room1)
(free robot1 rgripper1)
(free robot1 lgripper1)
(at ball1 room2)
(at ball2 room2)
(at ball3 room2)
(at ball4 room2)
(at ball5 room2)
(at ball6 room1)
)
(:goal
(and
(at ball1 room2)
(at ball2 room2)
(at ball3 room1)
(at ball4 room1)
(at ball5 room2)
(at ball6 room2)
)
)
)
