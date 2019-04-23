This engine aims to allow the user to visualise various AI behaviour types - how they interact with their environment, what is used in their "decision making", and to test adjustments to those AI in real-time.

This particular video demonstrates displaying the various vectors that we are interested in. I have currently shown the following velocities of interest:

- Current: This is the current velocity that the creature has. As the camera is aligned to this velocity, it will always point directly ahead (i.e. this is the vantage point of the creature assuming it is always looking exactly where it is going).

- Desired: This is the desired velocity that the creature wants to be moving at, given all the various "decisions" that the creature is making. The desired velocity minus the current velocity is the force that we need to apply to the creature to make it move towards its desired velocity.

There are three forces acting on this creature that vary depending on the situation it finds itself in - note that the creature can only "see" other creatures in front of it, all other creatures are ignored:

- Align: This is the force that is applied to the creature to keep it aligned to the same direction that its friends are travelling in. It is determined based on the average velocity of the surrounding creatures of the same type.

- Seek (or Cohesion for group cases such as this): This is the force that is being applied to the creature to make it travel to the average location of nearby creatures of the same type. It's easy to see that the red dot is in the middle of neaby creatures.

- Flee (or a simple version of Separation for group cases): This force causes the creature to move away from any nearby creatures. It increases by the inverse of the squared distance to neaby objects i.e. very quickly increases as other creatures come nearby, but conversly reduces very quickly as those creatures move away.

There is another "random" force that I've included and will display separately on the next update. I've also added in some quality of life enhancements, including the ability to pause the simulation and incrementally move the simulation forward by one time-step.

The next updates will be (in no particular order):

- Addition of more behaviours (e.g. pursuit / evade, wall avoidance)

- The ability to turn on / off behaviours - The ability to adjust the intensity of the various behaviours

- Addition of "predators"

- Ability to view the vantage point of different creatures

- And any others that you may have in mind! Please let me know!!

Please find the current codebase at https://github.com/s0lly/Steering-Beh... - but note that this is still a work in progress.

Hopefully this project will allow for the quick creation of multiple AI types by allowing live-editing of various parameters, and give a very deep sense into how the various AIs would respond to different stimuli.

Please let me know how if you have any suggestions on things to add!


Acknowledgements:

This was made using the Chili Framework: http://wiki.planetchili.net/index.php... - thanks Chili!

The ideas of using steering behaviours came about while reading Mat Buckland's book "Programming Game AI by Example", which has proved invaluable in building this visualisation.