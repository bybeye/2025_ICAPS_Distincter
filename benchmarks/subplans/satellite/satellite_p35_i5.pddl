

(define (problem satellite-35)
 (:domain satellite)
 (:objects 
    sat1 sat2 sat3 sat4 sat5 - satellite
    ins1 ins2 ins3 ins4 ins5 ins6 ins7 ins8 - instrument
    mod1 - mode
    dir1 dir2 dir3 dir4 dir5 dir6 - direction
    )
 (:init 
    (pointing sat1 dir6)
    (pointing sat2 dir4)
    (pointing sat3 dir6)
    (pointing sat4 dir6)
    (pointing sat5 dir5)
    (power_avail sat1)
    (power_avail sat2)
    (power_avail sat3)
    (power_avail sat4)
    (power_avail sat5)
    (calibration_target ins1 dir1)
    (calibration_target ins2 dir5)
    (calibration_target ins3 dir5)
    (calibration_target ins4 dir2)
    (calibration_target ins5 dir4)
    (calibration_target ins6 dir4)
    (calibration_target ins7 dir3)
    (calibration_target ins8 dir6)
    (on_board ins1 sat1)
    (on_board ins2 sat2)
    (on_board ins3 sat5)
    (on_board ins4 sat4)
    (on_board ins5 sat3)
    (on_board ins6 sat2)
    (on_board ins7 sat5)
    (on_board ins8 sat1)
    (supports ins3 mod1)
    (supports ins7 mod1)
    (supports ins2 mod1)
    (supports ins8 mod1)
    (supports ins4 mod1)
    (supports ins6 mod1)
    (supports ins5 mod1)
    (supports ins1 mod1))
 (:goal (and 
(pointing sat1 dir5)
))
)
