

(define (problem satellite-19)
 (:domain satellite)
 (:objects 
    sat1 sat2 sat3 - satellite
    ins1 ins2 ins3 ins4 - instrument
    mod1 - mode
    dir1 dir2 dir3 dir4 - direction
    )
 (:init 
    (pointing sat1 dir3)
    (pointing sat2 dir1)
    (pointing sat3 dir4)
    (power_avail sat1)
    (power_avail sat2)
    (power_avail sat3)
    (calibration_target ins1 dir3)
    (calibration_target ins2 dir4)
    (calibration_target ins3 dir1)
    (calibration_target ins4 dir2)
    (on_board ins1 sat3)
    (on_board ins2 sat2)
    (on_board ins3 sat1)
    (on_board ins4 sat1)
    (supports ins4 mod1)
    (supports ins1 mod1)
    (supports ins2 mod1)
    (supports ins3 mod1))
 (:goal (and 
(have_image dir4 mod1)
))
)
