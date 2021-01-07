"""
Overall control for 2048 clone 512.  Coordinates
model and view and implements controller
functionality by interpreting keyboard input
"""
import model
import view
import keypress
import sys
from model import Vec


def main():
    # Set up model component
    grid = model.Board()
    # Set up view component
    game_view = view.GameView(600, 600)
    grid_view = view.GridView(game_view, len(grid.tiles))
    grid.add_listener(grid_view)
    # Handle control component responsibility here
    commands = keypress.Command(game_view)


    grid.place_tile()
    grid.place_tile()
    # Game continues until there is no empty
    # space for a tile
    while grid.has_empty():

        cmd = commands.next()
        if cmd == keypress.LEFT:
            grid._move(Vec(0,-1))
        elif cmd == keypress.RIGHT:
            grid._move(Vec(0,1))
        elif cmd == keypress.UP:
            grid._move(Vec(-1,0))
        elif cmd == keypress.DOWN:
            grid._move(Vec(1,0))
        elif cmd == keypress.CLOSE:
            # Ended game by closing window
            print(f"Your score: {grid.score()}")
            sys.exit(0)
        else: 
            assert cmd == keypress.UNMAPPED

        if grid.effective():
            grid.place_tile()

    game_view.lose(grid.score())


if __name__ == "__main__":
    main()
