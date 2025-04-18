(define (problem satellite-10)
 (:domain satellite)
 (:objects 
    sat1 sat2 - satellite
    ins1 ins2 - instrument
    mod1 - mode
    dir1 dir2 dir3 - direction
    )
 (:init 
    (pointing sat1 dir3)
    (pointing sat2 dir1)
    (power_avail sat1)
    (power_avail sat2)
    (calibration_target ins1 dir1)
    (calibration_target ins2 dir3)
    (on_board ins1 sat1)
    (on_board ins2 sat2)
    (supports ins2 mod1)
    (supports ins1 mod1))
 (:goal (and 
(have_image dir1 mod1)
))
)
