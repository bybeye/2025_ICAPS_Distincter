

(define (problem satellite-61)
 (:domain satellite)
 (:objects 
    sat1 sat2 sat3 sat4 sat5 sat6 sat7 - satellite
    ins1 ins2 ins3 ins4 ins5 ins6 ins7 ins8 ins9 ins10 ins11 ins12 ins13 - instrument
    mod1 mod2 - mode
    dir1 dir2 dir3 dir4 dir5 dir6 dir7 dir8 - direction
    )
 (:init 
    (pointing sat1 dir3)
    (pointing sat2 dir2)
    (pointing sat3 dir5)
    (pointing sat4 dir5)
    (pointing sat5 dir7)
    (pointing sat6 dir1)
    (pointing sat7 dir8)
    (power_avail sat1)
    (power_avail sat2)
    (power_avail sat3)
    (power_avail sat4)
    (power_avail sat5)
    (power_avail sat6)
    (power_avail sat7)
    (calibration_target ins1 dir6)
    (calibration_target ins2 dir5)
    (calibration_target ins3 dir7)
    (calibration_target ins4 dir5)
    (calibration_target ins5 dir1)
    (calibration_target ins6 dir7)
    (calibration_target ins7 dir1)
    (calibration_target ins8 dir7)
    (calibration_target ins9 dir6)
    (calibration_target ins10 dir8)
    (calibration_target ins11 dir6)
    (calibration_target ins12 dir4)
    (calibration_target ins13 dir1)
    (on_board ins1 sat5)
    (on_board ins2 sat6)
    (on_board ins3 sat1)
    (on_board ins4 sat3)
    (on_board ins5 sat2)
    (on_board ins6 sat7)
    (on_board ins7 sat4)
    (on_board ins8 sat6)
    (on_board ins9 sat3)
    (on_board ins10 sat4)
    (on_board ins11 sat6)
    (on_board ins12 sat3)
    (on_board ins13 sat7)
    (supports ins8 mod1)
    (supports ins10 mod2)
    (supports ins6 mod1)
    (supports ins8 mod2)
    (supports ins11 mod2)
    (supports ins4 mod1)
    (supports ins7 mod1)
    (supports ins5 mod2)
    (supports ins10 mod1)
    (supports ins13 mod2)
    (supports ins3 mod1)
    (supports ins2 mod2)
    (supports ins3 mod2)
    (supports ins9 mod1)
    (supports ins12 mod2)
    (supports ins12 mod1)
    (supports ins4 mod2)
    (supports ins5 mod1)
    (supports ins1 mod1))
 (:goal (and 
(have_image dir1 mod1)
(have_image dir1 mod2)
(have_image dir2 mod1)
(have_image dir3 mod1)
(have_image dir4 mod1)
))
)
