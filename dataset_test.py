from backend.kaggle import get_dataframe

sudoku_df = get_dataframe()

puzzle = sudoku_df['puzzle'].to_list()

# Print 10 random puzzles
# Must be random, use randomness
import random
random_puzzles = random.sample(puzzle, 10)
print(random_puzzles)