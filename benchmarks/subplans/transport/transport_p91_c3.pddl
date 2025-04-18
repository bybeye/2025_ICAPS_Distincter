

(define (problem transport-91)
 (:domain transport)
 (:objects 
    v1 v2 v3 v4 v5 v6 v7 - vehicle
    p1 p2 p3 p4 p5 p6 p7 p8 p9 p10 p11 p12 p13 p14 p15 p16 - package
    l1 l2 l3 l4 l5 l6 l7 l8 l9 l10 l11 l12 l13 l14 l15 l16 - location
    c0 c1 c2 c3 - size
    )
 (:init (capacity v1 c2)
    (capacity v2 c1)
    (capacity v3 c2)
    (capacity v4 c1)
    (capacity v5 c1)
    (capacity v6 c1)
    (capacity v7 c2)
    (capacity-predecessor c0 c1)
    (capacity-predecessor c1 c2)
    (capacity-predecessor c2 c3)
    (at p1 l4)
    (at p2 l1)
    (at p3 l8)
    (at p4 l15)
    (at p5 l9)
    (at p6 l7)
    (at p7 l15)
    (at p8 l6)
    (at p9 l13)
    (at p10 l8)
    (at p11 l16)
    (at p12 l12)
    (at p13 l13)
    (at p14 l11)
    (at p15 l7)
    (at p16 l3)
    (at v1 l2)
    (at v2 l12)
    (at v3 l3)
    (at v4 l5)
    (at v5 l12)
    (at v6 l3)
    (at v7 l10)
    (road l12 l7)
    (road l3 l1)
    (road l14 l7)
    (road l5 l1)
    (road l5 l7)
    (road l4 l15)
    (road l9 l5)
    (road l1 l6)
    (road l1 l3)
    (road l2 l8)
    (road l10 l15)
    (road l15 l5)
    (road l16 l10)
    (road l8 l2)
    (road l5 l9)
    (road l5 l15)
    (road l2 l7)
    (road l1 l5)
    (road l15 l4)
    (road l11 l16)
    (road l13 l1)
    (road l6 l1)
    (road l15 l10)
    (road l7 l12)
    (road l10 l16)
    (road l1 l13)
    (road l7 l2)
    (road l7 l5)
    (road l16 l11)
    (road l7 l14)
    (road l7 l16)
    (road l16 l7)
    (road l2 l12)
    (road l12 l2)
    (road l5 l16)
    (road l16 l5)
    (road l6 l16)
    (road l16 l6)
    (road l6 l8)
    (road l8 l6)
    (road l3 l16)
    (road l16 l3)
    (road l9 l15)
    (road l15 l9)
    (road l3 l5)
    (road l5 l3)
    (road l9 l11)
    (road l11 l9)
    (road l5 l11)
    (road l11 l5)
    (road l3 l8)
    (road l8 l3)
    (road l3 l10)
    (road l10 l3)
    (road l1 l12)
    (road l12 l1)
    (road l12 l14)
    (road l14 l12)
    (road l1 l2)
    (road l2 l1)
    (road l6 l7)
    (road l7 l6)
    (road l4 l12)
    (road l12 l4)
    (road l14 l16)
    (road l16 l14)
    (road l7 l11)
    (road l11 l7)
    (road l4 l9)
    (road l9 l4)
    (road l14 l15)
    (road l15 l14)
    (road l4 l7)
    (road l7 l4)
    (road l11 l14)
    (road l14 l11)
    (road l7 l10)
    (road l10 l7)
    (road l1 l10)
    (road l10 l1)
    (road l1 l16)
    (road l16 l1)
    (road l8 l11)
    (road l11 l8)
    (road l2 l16)
    (road l16 l2)
    (road l2 l3)
    (road l3 l2)
    (road l8 l14)
    (road l14 l8)
    (road l4 l5)
    (road l5 l4)
    (road l9 l16)
    (road l16 l9)
    (road l13 l14)
    (road l14 l13)
    (road l10 l13)
    (road l13 l10)
    (road l12 l16)
    (road l16 l12)
    (road l3 l12)
    (road l12 l3)
    (road l8 l10)
    (road l10 l8)
    (road l5 l8)
    (road l8 l5)
    (road l8 l12)
    (road l12 l8)
    (road l3 l6)
    (road l6 l3)
    (road l2 l14)
    (road l14 l2)
    (road l1 l14)
    (road l14 l1)
    (road l4 l10)
    (road l10 l4)
    (road l4 l11)
    (road l11 l4)
    (road l9 l13)
    (road l13 l9)
    (road l10 l11)
    (road l11 l10)
    (road l1 l11)
    (road l11 l1)
    (road l1 l8)
    (road l8 l1)
    (road l4 l14)
    (road l14 l4)
    (road l4 l8)
    (road l8 l4)
    (road l1 l9)
    (road l9 l1)
    (road l3 l13)
    (road l13 l3)
    (road l11 l12)
    (road l12 l11)
    (road l12 l13)
    (road l13 l12)
    (road l11 l13)
    (road l13 l11)
    (road l3 l15)
    (road l15 l3)
    (road l2 l4)
    (road l4 l2)
    (road l6 l9)
    (road l9 l6)
    (road l4 l13)
    (road l13 l4)
    (road l5 l13)
    (road l13 l5)
    (road l9 l14)
    (road l14 l9)
    (road l10 l12)
    (road l12 l10)
    (road l4 l16)
    (road l16 l4)
    (road l4 l6)
    (road l6 l4)
    (road l13 l16)
    (road l16 l13)
    (road l7 l9)
    (road l9 l7)
    (road l2 l15)
    (road l15 l2)
    (road l5 l6)
    (road l6 l5)
    (road l9 l10)
    (road l10 l9)
    (road l6 l11)
    (road l11 l6)
    (road l7 l8)
    (road l8 l7)
    (road l15 l16)
    (road l16 l15)
    (road l6 l13)
    (road l13 l6)
    (road l6 l15)
    (road l15 l6)
    (road l1 l15)
    (road l15 l1)
    (road l5 l12)
    (road l12 l5)
    (road l8 l9)
    (road l9 l8)
    (road l10 l14)
    (road l14 l10)
    (road l6 l14)
    (road l14 l6)
    (road l11 l15)
    (road l15 l11)
    (road l2 l6)
    (road l6 l2)
    (road l3 l9)
    (road l9 l3)
    (road l1 l7)
    (road l7 l1)
    (road l2 l10)
    (road l10 l2)
    (road l2 l9)
    (road l9 l2)
    (road l7 l13)
    (road l13 l7)
    (road l6 l10)
    (road l10 l6)
    (road l3 l14)
    (road l14 l3)
    (road l8 l15)
    (road l15 l8)
    (road l3 l4)
    (road l4 l3)
    (road l5 l10)
    (road l10 l5)
    (road l7 l15)
    (road l15 l7)
    (road l9 l12)
    (road l12 l9)
    (road l6 l12)
    (road l12 l6)
    (road l1 l4)
    (road l4 l1)
    (road l3 l11)
    (road l11 l3)
    (road l8 l16)
    (road l16 l8)
    (road l5 l14)
    (road l14 l5)
    (road l2 l5)
    (road l5 l2)
    (road l8 l13)
    (road l13 l8)
    (road l12 l15)
    (road l15 l12)
    (road l2 l13)
    (road l13 l2)
    (road l13 l15)
    (road l15 l13)
    (road l2 l11)
    (road l11 l2)
    (road l3 l7)
    (road l7 l3)
    )
 (:goal (and 
(at p1 l2)
(at p2 l3)
(at p3 l6)
(at p4 l4)
))
)
