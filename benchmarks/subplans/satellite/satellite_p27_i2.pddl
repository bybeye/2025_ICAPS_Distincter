

(define (problem satellite-27)
 (:domain satellite)
 (:objects 
    sat1 sat2 sat3 sat4 - satellite
    ins1 ins2 ins3 ins4 ins5 ins6 - instrument
    mod1 - mode
    dir1 dir2 dir3 dir4 dir5 - direction
    )
 (:init 
    (pointing sat1 dir3)
    (pointing sat2 dir3)
    (pointing sat3 dir5)
    (pointing sat4 dir2)
    (power_avail sat1)
    (power_avail sat2)
    (power_avail sat3)
    (power_avail sat4)
    (calibration_target ins1 dir3)
    (calibration_target ins2 dir2)
    (calibration_target ins3 dir4)
    (calibration_target ins4 dir4)
    (calibration_target ins5 dir3)
    (calibration_target ins6 dir1)
    (on_board ins1 sat2)
    (on_board ins2 sat4)
    (on_board ins3 sat3)
    (on_board ins4 sat1)
    (on_board ins5 sat2)
    (on_board ins6 sat4)
    (supports ins3 mod1)
    (supports ins4 mod1)
    (supports ins5 mod1)
    (supports ins6 mod1)
    (supports ins2 mod1)
    (supports ins1 mod1))
 (:goal (and 
(have_image dir5 mod1)
))
)
