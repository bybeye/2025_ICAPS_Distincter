

(define (problem floortile-33)
 (:domain floortile)
 (:objects 
    tile_0_1
    tile_0_2
    tile_0_3
    tile_0_4
    tile_1_1
    tile_1_2
    tile_1_3
    tile_1_4
    tile_2_1
    tile_2_2
    tile_2_3
    tile_2_4
    tile_3_1
    tile_3_2
    tile_3_3
    tile_3_4
    tile_4_1
    tile_4_2
    tile_4_3
    tile_4_4
    tile_5_1
    tile_5_2
    tile_5_3
    tile_5_4 - tile
    robot1 - robot
    white black - color
)
 (:init 
    (robot-at robot1 tile_0_4)
    (robot-has robot1 white)
    (available-color white)
    (available-color black)
    (clear tile_0_1)
    (clear tile_0_2)
    (clear tile_0_3)
    (clear tile_1_1)
    (clear tile_1_2)
    (clear tile_1_3)
    (clear tile_1_4)
    (clear tile_2_1)
    (clear tile_2_2)
    (clear tile_2_3)
    (clear tile_2_4)
    (clear tile_3_1)
    (clear tile_3_2)
    (clear tile_3_3)
    (clear tile_3_4)
    (clear tile_4_1)
    (clear tile_4_2)
    (clear tile_4_3)
    (clear tile_4_4)
    (clear tile_5_1)
    (clear tile_5_2)
    (clear tile_5_3)
    (clear tile_5_4)
    (up tile_1_1 tile_0_1 )
    (up tile_1_2 tile_0_2 )
    (up tile_1_3 tile_0_3 )
    (up tile_1_4 tile_0_4 )
    (up tile_2_1 tile_1_1 )
    (up tile_2_2 tile_1_2 )
    (up tile_2_3 tile_1_3 )
    (up tile_2_4 tile_1_4 )
    (up tile_3_1 tile_2_1 )
    (up tile_3_2 tile_2_2 )
    (up tile_3_3 tile_2_3 )
    (up tile_3_4 tile_2_4 )
    (up tile_4_1 tile_3_1 )
    (up tile_4_2 tile_3_2 )
    (up tile_4_3 tile_3_3 )
    (up tile_4_4 tile_3_4 )
    (up tile_5_1 tile_4_1 )
    (up tile_5_2 tile_4_2 )
    (up tile_5_3 tile_4_3 )
    (up tile_5_4 tile_4_4 )
    (down tile_0_1 tile_1_1 )
    (down tile_0_2 tile_1_2 )
    (down tile_0_3 tile_1_3 )
    (down tile_0_4 tile_1_4 )
    (down tile_1_1 tile_2_1 )
    (down tile_1_2 tile_2_2 )
    (down tile_1_3 tile_2_3 )
    (down tile_1_4 tile_2_4 )
    (down tile_2_1 tile_3_1 )
    (down tile_2_2 tile_3_2 )
    (down tile_2_3 tile_3_3 )
    (down tile_2_4 tile_3_4 )
    (down tile_3_1 tile_4_1 )
    (down tile_3_2 tile_4_2 )
    (down tile_3_3 tile_4_3 )
    (down tile_3_4 tile_4_4 )
    (down tile_4_1 tile_5_1 )
    (down tile_4_2 tile_5_2 )
    (down tile_4_3 tile_5_3 )
    (down tile_4_4 tile_5_4 )
    (left tile_0_1 tile_0_2 )
    (left tile_0_2 tile_0_3 )
    (left tile_0_3 tile_0_4 )
    (left tile_1_1 tile_1_2 )
    (left tile_1_2 tile_1_3 )
    (left tile_1_3 tile_1_4 )
    (left tile_2_1 tile_2_2 )
    (left tile_2_2 tile_2_3 )
    (left tile_2_3 tile_2_4 )
    (left tile_3_1 tile_3_2 )
    (left tile_3_2 tile_3_3 )
    (left tile_3_3 tile_3_4 )
    (left tile_4_1 tile_4_2 )
    (left tile_4_2 tile_4_3 )
    (left tile_4_3 tile_4_4 )
    (left tile_5_1 tile_5_2 )
    (left tile_5_2 tile_5_3 )
    (left tile_5_3 tile_5_4 )
    (right tile_0_2 tile_0_1 )
    (right tile_0_3 tile_0_2 )
    (right tile_0_4 tile_0_3 )
    (right tile_1_2 tile_1_1 )
    (right tile_1_3 tile_1_2 )
    (right tile_1_4 tile_1_3 )
    (right tile_2_2 tile_2_1 )
    (right tile_2_3 tile_2_2 )
    (right tile_2_4 tile_2_3 )
    (right tile_3_2 tile_3_1 )
    (right tile_3_3 tile_3_2 )
    (right tile_3_4 tile_3_3 )
    (right tile_4_2 tile_4_1 )
    (right tile_4_3 tile_4_2 )
    (right tile_4_4 tile_4_3 )
    (right tile_5_2 tile_5_1 )
    (right tile_5_3 tile_5_2 )
    (right tile_5_4 tile_5_3 ))
 (:goal (and 
(painted tile_5_1 white)
(painted tile_4_1 black)
(painted tile_3_1 white)
(painted tile_2_1 black)
(painted tile_1_1 white)
(painted tile_5_2 black)
(painted tile_4_2 white)
(painted tile_3_2 black)
(painted tile_2_2 white)
(painted tile_1_2 black)
(painted tile_5_3 white)
(painted tile_4_3 black)
(painted tile_3_3 white)
(painted tile_2_3 black)
(painted tile_1_3 white)
(painted tile_5_4 black)
(painted tile_4_4 white)
(painted tile_3_4 black)
))
)
