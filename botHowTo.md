# Adding a Bot to Jonkler Tanks

## Key Functions for Bot Implementation

The bot in Jonkler Tanks is implemented through the `botMain` function, which is called when a bot is active in the game. This function is where the bot's behavior logic should be defined. **Also checkout `src/game/bot.c` and `src/game/math.c` for more details**. Below are the key functions available for controlling the bot:

1. **shoot**  
   Performs a shot. Use this function to enable the bot to attack.

2. **recalcPlayerPos**  
   Recalculates the tank's position. This function must be called whenever changes occur on the map near the tank to update its position.

3. **calcHitPosition**  
   Returns information about the projectile's hit:
   - `-1`: The projectile went out of the map.
   - Negative value: The projectile hit an opponent's collision.
   - Positive value: The coordinate on the field where the projectile landed.

4. **smoothChangeAngle(endAngle)**  
   Smoothly adjusts the tank's rotation angle to the specified `endAngle`.

5. **smoothChangePower(endPower)**  
   Smoothly adjusts the shot power to the specified `endPower`.

6. **smoothMove(direction, isFirstPlayer)**  
   Moves the tank model in the specified direction. Takes two parameters:
   - `direction`: The direction of movement.
   - `isFirstPlayer`: A boolean indicating whether the current player is the first (left) player.

## Steps to Implement a Bot

1. **Define the Bot's Strategy**  
   Decide how the bot will behave: how it will select targets, when to shoot, how to move, etc. For example, the bot can:
   - Analyze the map and the opponent's position.
   - Choose the optimal angle and power for shots.
   - Move to dodge shots or take advantageous positions.

2. **Implement Logic in `botMain`**  
   In the `botMain` function, define the bot's core logic. For example:
   - Use `calcHitPosition` to check where the projectile will land with current parameters.
   - Call `smoothChangeAngle` and `smoothChangePower` to adjust the angle and power of the shot.
   - Use `smoothMove` to move the tank.
   - Call `shoot` when the bot is ready to fire.
   - Call `recalcPlayerPos` after any map changes (e.g., after a shot).