# GrappleGame

Project files: https://github.com/Saish-in3d/GrappleGame

Project has been built in UE5.0


Overview: A grapple gun is incorporated with Physics based character. Demo has been optimised for
mobile devices with touch inputs as mentioned in the assignment guidelines.


Physics: When a force is applied on the main body, it gains momentum. When the character grapples
to a surface, with the help of physics constraints, main body attaches to the surface thus creating a
mechanical couple which ultimately helps the main body to generate radial centrifugal force thus
creating a sling shot movement. Couple force can be negated by moving the joystick in opposite
direction of the swing.


Controls: Grapple button has been placed on the top of the screen which enables player to play with
three fingers ultimately reducing player input delay while grappling.


Further Improvements:

1. PhysicsConstraintComponent creation at start was planned in the beginning, but it resulted in
multiple bugs while testing on the mobile. Creating a new component every time while
grappling solved the issue.
3. PawnMovementComponent based grapple mechanics: Forces can be applied on the main
body using logic which when implemented can completely remove usage of
PhysicsConstraintComponent thus improving overall performance.
-regards
