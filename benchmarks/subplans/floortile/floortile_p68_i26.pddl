

(define (problem floortile-68)
 (:domain floortile)
 (:objects 
    tile_0_1
    tile_0_2
    tile_0_3
    tile_0_4
    tile_0_5
    tile_0_6
    tile_0_7
    tile_0_8
    tile_0_9
    tile_1_1
    tile_1_2
    tile_1_3
    tile_1_4
    tile_1_5
    tile_1_6
    tile_1_7
    tile_1_8
    tile_1_9
    tile_2_1
    tile_2_2
    tile_2_3
    tile_2_4
    tile_2_5
    tile_2_6
    tile_2_7
    tile_2_8
    tile_2_9
    tile_3_1
    tile_3_2
    tile_3_3
    tile_3_4
    tile_3_5
    tile_3_6
    tile_3_7
    tile_3_8
    tile_3_9 - tile
    robot1
    robot2 - robot
    white black - color
)
 (:init 
    (robot-at robot1 tile_0_5)
    (robot-has robot1 white)
    (robot-at robot2 tile_0_4)
    (robot-has robot2 white)
    (available-color white)
    (available-color black)
    (clear tile_0_1)
    (clear tile_0_2)
    (clear tile_0_3)
    (clear tile_0_6)
    (clear tile_0_7)
    (clear tile_0_8)
    (clear tile_0_9)
    (clear tile_1_1)
    (clear tile_1_2)
    (clear tile_1_3)
    (clear tile_1_4)
    (clear tile_1_5)
    (clear tile_1_6)
    (clear tile_1_7)
    (clear tile_1_8)
    (clear tile_1_9)
    (clear tile_2_1)
    (clear tile_2_2)
    (clear tile_2_3)
    (clear tile_2_4)
    (clear tile_2_5)
    (clear tile_2_6)
    (clear tile_2_7)
    (clear tile_2_8)
    (clear tile_2_9)
    (clear tile_3_1)
    (clear tile_3_2)
    (clear tile_3_3)
    (clear tile_3_4)
    (clear tile_3_5)
    (clear tile_3_6)
    (clear tile_3_7)
    (clear tile_3_8)
    (clear tile_3_9)
    (up tile_1_1 tile_0_1 )
    (up tile_1_2 tile_0_2 )
    (up tile_1_3 tile_0_3 )
    (up tile_1_4 tile_0_4 )
    (up tile_1_5 tile_0_5 )
    (up tile_1_6 tile_0_6 )
    (up tile_1_7 tile_0_7 )
    (up tile_1_8 tile_0_8 )
    (up tile_1_9 tile_0_9 )
    (up tile_2_1 tile_1_1 )
    (up tile_2_2 tile_1_2 )
    (up tile_2_3 tile_1_3 )
    (up tile_2_4 tile_1_4 )
    (up tile_2_5 tile_1_5 )
    (up tile_2_6 tile_1_6 )
    (up tile_2_7 tile_1_7 )
    (up tile_2_8 tile_1_8 )
    (up tile_2_9 tile_1_9 )
    (up tile_3_1 tile_2_1 )
    (up tile_3_2 tile_2_2 )
    (up tile_3_3 tile_2_3 )
    (up tile_3_4 tile_2_4 )
    (up tile_3_5 tile_2_5 )
    (up tile_3_6 tile_2_6 )
    (up tile_3_7 tile_2_7 )
    (up tile_3_8 tile_2_8 )
    (up tile_3_9 tile_2_9 )
    (down tile_0_1 tile_1_1 )
    (down tile_0_2 tile_1_2 )
    (down tile_0_3 tile_1_3 )
    (down tile_0_4 tile_1_4 )
    (down tile_0_5 tile_1_5 )
    (down tile_0_6 tile_1_6 )
    (down tile_0_7 tile_1_7 )
    (down tile_0_8 tile_1_8 )
    (down tile_0_9 tile_1_9 )
    (down tile_1_1 tile_2_1 )
    (down tile_1_2 tile_2_2 )
    (down tile_1_3 tile_2_3 )
    (down tile_1_4 tile_2_4 )
    (down tile_1_5 tile_2_5 )
    (down tile_1_6 tile_2_6 )
    (down tile_1_7 tile_2_7 )
    (down tile_1_8 tile_2_8 )
    (down tile_1_9 tile_2_9 )
    (down tile_2_1 tile_3_1 )
    (down tile_2_2 tile_3_2 )
    (down tile_2_3 tile_3_3 )
    (down tile_2_4 tile_3_4 )
    (down tile_2_5 tile_3_5 )
    (down tile_2_6 tile_3_6 )
    (down tile_2_7 tile_3_7 )
    (down tile_2_8 tile_3_8 )
    (down tile_2_9 tile_3_9 )
    (left tile_0_1 tile_0_2 )
    (left tile_0_2 tile_0_3 )
    (left tile_0_3 tile_0_4 )
    (left tile_0_4 tile_0_5 )
    (left tile_0_5 tile_0_6 )
    (left tile_0_6 tile_0_7 )
    (left tile_0_7 tile_0_8 )
    (left tile_0_8 tile_0_9 )
    (left tile_1_1 tile_1_2 )
    (left tile_1_2 tile_1_3 )
    (left tile_1_3 tile_1_4 )
    (left tile_1_4 tile_1_5 )
    (left tile_1_5 tile_1_6 )
    (left tile_1_6 tile_1_7 )
    (left tile_1_7 tile_1_8 )
    (left tile_1_8 tile_1_9 )
    (left tile_2_1 tile_2_2 )
    (left tile_2_2 tile_2_3 )
    (left tile_2_3 tile_2_4 )
    (left tile_2_4 tile_2_5 )
    (left tile_2_5 tile_2_6 )
    (left tile_2_6 tile_2_7 )
    (left tile_2_7 tile_2_8 )
    (left tile_2_8 tile_2_9 )
    (left tile_3_1 tile_3_2 )
    (left tile_3_2 tile_3_3 )
    (left tile_3_3 tile_3_4 )
    (left tile_3_4 tile_3_5 )
    (left tile_3_5 tile_3_6 )
    (left tile_3_6 tile_3_7 )
    (left tile_3_7 tile_3_8 )
    (left tile_3_8 tile_3_9 )
    (right tile_0_2 tile_0_1 )
    (right tile_0_3 tile_0_2 )
    (right tile_0_4 tile_0_3 )
    (right tile_0_5 tile_0_4 )
    (right tile_0_6 tile_0_5 )
    (right tile_0_7 tile_0_6 )
    (right tile_0_8 tile_0_7 )
    (right tile_0_9 tile_0_8 )
    (right tile_1_2 tile_1_1 )
    (right tile_1_3 tile_1_2 )
    (right tile_1_4 tile_1_3 )
    (right tile_1_5 tile_1_4 )
    (right tile_1_6 tile_1_5 )
    (right tile_1_7 tile_1_6 )
    (right tile_1_8 tile_1_7 )
    (right tile_1_9 tile_1_8 )
    (right tile_2_2 tile_2_1 )
    (right tile_2_3 tile_2_2 )
    (right tile_2_4 tile_2_3 )
    (right tile_2_5 tile_2_4 )
    (right tile_2_6 tile_2_5 )
    (right tile_2_7 tile_2_6 )
    (right tile_2_8 tile_2_7 )
    (right tile_2_9 tile_2_8 )
    (right tile_3_2 tile_3_1 )
    (right tile_3_3 tile_3_2 )
    (right tile_3_4 tile_3_3 )
    (right tile_3_5 tile_3_4 )
    (right tile_3_6 tile_3_5 )
    (right tile_3_7 tile_3_6 )
    (right tile_3_8 tile_3_7 )
    (right tile_3_9 tile_3_8 ))
 (:goal (and 
(painted tile_1_9 white)
))
)
