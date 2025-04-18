

(define (problem satellite-32)
 (:domain satellite)
 (:objects 
    sat1 sat2 sat3 sat4 sat5 - satellite
    ins1 ins2 ins3 ins4 ins5 ins6 ins7 - instrument
    mod1 - mode
    dir1 dir2 dir3 dir4 dir5 - direction
    )
 (:init 
    (pointing sat1 dir4)
    (pointing sat2 dir3)
    (pointing sat3 dir3)
    (pointing sat4 dir5)
    (pointing sat5 dir2)
    (power_avail sat1)
    (power_avail sat2)
    (power_avail sat3)
    (power_avail sat4)
    (power_avail sat5)
    (calibration_target ins1 dir4)
    (calibration_target ins2 dir4)
    (calibration_target ins3 dir5)
    (calibration_target ins4 dir4)
    (calibration_target ins5 dir5)
    (calibration_target ins6 dir3)
    (calibration_target ins7 dir4)
    (on_board ins1 sat4)
    (on_board ins2 sat5)
    (on_board ins3 sat3)
    (on_board ins4 sat1)
    (on_board ins5 sat2)
    (on_board ins6 sat3)
    (on_board ins7 sat2)
    (supports ins3 mod1)
    (supports ins7 mod1)
    (supports ins2 mod1)
    (supports ins1 mod1)
    (supports ins5 mod1)
    (supports ins4 mod1)
    (supports ins6 mod1))
 (:goal (and 
(have_image dir1 mod1)
(have_image dir2 mod1)
(have_image dir3 mod1)
(have_image dir4 mod1)
))
)
