import kagglehub
import pandas as pd

# Global variables to store the dataset
_df = None  # This will store the loaded dataset

def get_dataframe():
    global _df
    if _df is None:  # Only load the dataset once
        print("Loading dataset...")
        path = kagglehub.dataset_download("rohanrao/sudoku")
        _df = pd.read_csv(path + "/sudoku.csv")
    return _df
