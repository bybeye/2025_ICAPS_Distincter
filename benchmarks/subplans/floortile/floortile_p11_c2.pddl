

(define (problem floortile-11)
 (:domain floortile)
 (:objects
    tile_0_1
    tile_0_2
    tile_1_1
    tile_1_2
    tile_2_1
    tile_2_2 - tile
    robot1 robot2 - robot
    white black - color
)
 (:init
    (robot-at robot1 tile_2_1)
    (robot-at robot2 tile_1_2)
    (robot-has robot1 white)
    (robot-has robot2 black)
    (available-color white)
    (available-color black)
    (clear tile_0_1)
    (clear tile_0_2)
    (clear tile_1_1)
    (clear tile_2_2)
    (up tile_1_1 tile_0_1 )
    (up tile_1_2 tile_0_2 )
    (up tile_2_1 tile_1_1 )
    (up tile_2_2 tile_1_2 )
    (down tile_0_1 tile_1_1 )
    (down tile_0_2 tile_1_2 )
    (down tile_1_1 tile_2_1 )
    (down tile_1_2 tile_2_2 )
    (left tile_0_1 tile_0_2 )
    (left tile_1_1 tile_1_2 )
    (left tile_2_1 tile_2_2 )
    (right tile_0_2 tile_0_1 )
    (right tile_1_2 tile_1_1 )
    (right tile_2_2 tile_2_1 ))
 (:goal (and 
(painted tile_2_1 black)
(painted tile_1_1 white)
(painted tile_2_2 white)
))
)
