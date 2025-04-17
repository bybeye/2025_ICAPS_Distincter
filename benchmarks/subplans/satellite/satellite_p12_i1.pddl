

(define (problem satellite-12)
 (:domain satellite)
 (:objects 
    sat1 sat2 sat3 - satellite
    ins1 ins2 ins3 - instrument
    mod1 - mode
    dir1 dir2 dir3 dir4 - direction
    )
 (:init 
    (pointing sat1 dir3)
    (pointing sat2 dir4)
    (pointing sat3 dir4)
    (power_avail sat1)
    (power_avail sat2)
    (power_avail sat3)
    (calibration_target ins1 dir3)
    (calibration_target ins2 dir1)
    (calibration_target ins3 dir3)
    (on_board ins1 sat3)
    (on_board ins2 sat1)
    (on_board ins3 sat2)
    (supports ins3 mod1)
    (supports ins2 mod1)
    (supports ins1 mod1))
 (:goal (and 
(have_image dir4 mod1)
))
)
