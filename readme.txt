This engine allows the user to input thousands of objects, and in using very simple code, create what appears to be a swarm of bees. This video demonstrates 1000 "creatures", running at a smooth enough frame rate.

This is my first attempt at "emergent behaviour" - i.e. AI that is coded with just a small number of commands, but that in a group appear to be controlled by some larger intelligence. I have "cheated" a little bit by adding in a couple of global toggles (using sin waves) that alter how far away other creatures need to be over time. This creates a nice "heartbeat" type feel to the swarm.

I'd imagine that this code could also be used to simulate other things like particles for a magic fireball in a game, or similar. I might just have to use this in one of my next games!

I've also included a "beacon" effect, which attracts the entire swarm - although I do want to work on that a bit more so that the beacons slowly attract the swarm if that effect is being aimed for.

It relies on only a few functions and objects to do all the work, so hopefully the code is quite easy to understand and follow.

The codebase can be found here: https://github.com/s0lly/Swarm-AI

Please let me know how I could improve this!


Acknowledgements:

This was made using the Chili Framework: http://wiki.planetchili.net/index.php... - thanks Chili!

The ideas of using simple states to create emergent behaviour came about while reading Mat Buckland's book Programming Game AI by Example. This code uses ideas similar to the Seek and Evade AI states.