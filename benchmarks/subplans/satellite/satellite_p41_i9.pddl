

(define (problem satellite-41)
 (:domain satellite)
 (:objects 
    sat1 sat2 sat3 sat4 sat5 - satellite
    ins1 ins2 ins3 ins4 ins5 ins6 ins7 ins8 ins9 - instrument
    mod1 mod2 - mode
    dir1 dir2 dir3 dir4 dir5 dir6 - direction
    )
 (:init 
    (pointing sat1 dir5)
    (pointing sat2 dir5)
    (pointing sat3 dir1)
    (pointing sat4 dir3)
    (pointing sat5 dir1)
    (power_avail sat1)
    (power_avail sat2)
    (power_avail sat3)
    (power_avail sat4)
    (power_avail sat5)
    (calibration_target ins1 dir3)
    (calibration_target ins2 dir2)
    (calibration_target ins3 dir2)
    (calibration_target ins4 dir3)
    (calibration_target ins5 dir1)
    (calibration_target ins6 dir4)
    (calibration_target ins7 dir4)
    (calibration_target ins8 dir5)
    (calibration_target ins9 dir3)
    (on_board ins1 sat1)
    (on_board ins2 sat3)
    (on_board ins3 sat5)
    (on_board ins4 sat2)
    (on_board ins5 sat4)
    (on_board ins6 sat5)
    (on_board ins7 sat5)
    (on_board ins8 sat1)
    (on_board ins9 sat5)
    (supports ins6 mod1)
    (supports ins8 mod1)
    (supports ins7 mod2)
    (supports ins3 mod2)
    (supports ins9 mod2)
    (supports ins5 mod1)
    (supports ins1 mod1)
    (supports ins6 mod2)
    (supports ins7 mod1)
    (supports ins9 mod1)
    (supports ins2 mod2)
    (supports ins4 mod1))
 (:goal (and 
(pointing sat3 dir1)
))
)
