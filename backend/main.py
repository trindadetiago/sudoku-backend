from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel
from typing import List, Dict, Any
import subprocess
import tempfile
import os

import sys
from pathlib import Path

# Add the project root to the sys.path
project_root = Path(__file__).resolve().parent.parent.parent
sys.path.append(str(project_root))

from .kaggle import get_dataframe

print("Iniciando...")
sudoku_df = get_dataframe()
print("Dataset analisado!")
puzzle = sudoku_df['puzzle'].to_list()
solution = sudoku_df['solution'].to_list()
print("Pronto para iniciar!")

app = FastAPI(title="Search Algorithms API", version="1.0")

app.add_middleware(
    CORSMiddleware,
    allow_origins=["http://localhost:3000"],  # Your frontend URL
    allow_credentials=True,
    allow_methods=["GET", "POST", "OPTIONS"],  # Allow the OPTIONS method
    allow_headers=["*"],  # Allow all headers
)

CENTRAL_SEARCH_EXEC = "./central_search"

class SearchRequest(BaseModel):
    target: str
    algorithms: List[str]  # List of algorithms to run (e.g., ["linear", "binary", "skip_list"])

# Output response model
class SearchResponse(BaseModel):
    results: Dict[str, Dict[str, Any]]  # Algorithm name to result mapping

def run_central_search(target: str, algorithms: List[str]):
    print(f"Running central search with target: {target} and algorithms: {algorithms}")

    # Create a temporary input file
    with tempfile.NamedTemporaryFile(mode="w", delete=False) as temp_file:
        input_file_path = temp_file.name
        # Write the numbers to the file, one per line
        for number in puzzle:
            temp_file.write(f"{number}\n")
        # Write the target number at the end
        temp_file.write(f"{target}\n")
        temp_file.flush()

    try:
        # Construct the arguments for the central_search executable
        algorithm_string = ",".join(algorithms)
        args = [CENTRAL_SEARCH_EXEC, input_file_path, algorithm_string]

        # Run the central search executable
        print(f"Executing: {args}")
        result = subprocess.run(args, capture_output=True, text=True)

        if result.returncode != 0:
            print("Execution failed with a non-zero exit code.")
            print("Standard Output:", result.stdout)
            print("Standard Error:", result.stderr)
            raise subprocess.CalledProcessError(result.returncode, args, output=result.stdout, stderr=result.stderr)

        print(f"Saída:\n{result.stdout}")

        # Parse the output from the central search executable
        output_lines = result.stdout.splitlines()
        responses = {}
        current_algo = None
        algo_results = {
            'index': -1,
            'iterations': 0,
            'time': 0,
            'memory': 0,
            'type': "",
            'output': "",
            'result': "Nenhum resultado encontrado"
        }

        for i, line in enumerate(output_lines):
            if "Search" in line:
                # New algorithm section
                if current_algo and algo_results:
                    responses[current_algo] = algo_results
                current_algo = line.split()[0].lower()  # Extract algorithm name
                algo_output = "\n".join(output_lines[i:i+4]) # Extract the output lines for this algorithm
                algo_results = {
                    'index': -1,
                    'iterations': 0,
                    'time': 0,
                    'memory': 0,
                    'type': current_algo,
                    'output': algo_output,
                    'result': "Nenhum resultado encontrado"
                }
            elif "Value" in line:
                if line == 'Value not found in the list.':
                    algo_results["index"] = -1
                else:
                    algo_results["index"] = int(line.split(":")[1].strip())
                    algo_results["result"] = solution[algo_results["index"]]
            elif "Iterations:" in line:
                algo_results["iterations"] = int(line.split(":")[1].strip())
            elif "Time taken:" in line:
                algo_results["time"] = line.split(":")[1].strip()
            elif "Memory used:" in line:
                algo_results["memory"] = line.split(":")[1].strip()

        if current_algo and algo_results:
            responses[current_algo] = algo_results
        print("RESPONSE:", responses)
        return responses

    except subprocess.CalledProcessError as e:
        print(f"Error during execution: {e}")
        print("Standard Output:", e.output)
        print("Standard Error:", e.stderr)
        raise

    finally:
        # Clean up: Remove the temporary input file
        if os.path.exists(input_file_path):
            os.remove(input_file_path)
            print(f"Temporary input file '{input_file_path}' deleted.")

@app.post("/search", response_model=SearchResponse)
def search(request: SearchRequest):
    # return {"results": {'linear': {'index': 2541323, 'iterations': 2541324, 'time': '57139 microseconds', 'memory': '0 KB', 'type': 'linear', 'output': 'Linear Search:\nValue found at index: 2541323\nIterations: 2541324\nTime taken: 57139 microseconds', 'result': '821675439345129678967834521739412856182956743654783912518367294293548167476291385'}, 'skip_list': {'index': 8333849, 'iterations': 23, 'time': '9 microseconds', 'memory': '0 KB', 'type': 'binary', 'output': 'Binary Search:\nValue found at index: 8333849\nIterations: 23\nTime taken: 9 microseconds', 'result': '251863947493175826786429513928614735174538692365792184842956371537281469619347258'}, 'binary': {'index': 8333849, 'iterations': 23, 'time': '9 microseconds', 'memory': '0 KB', 'type': 'binary', 'output': 'Binary Search:\nValue found at index: 8333849\nIterations: 23\nTime taken: 9 microseconds', 'result': '251863947493175826786429513928614735174538692365792184842956371537281469619347258'}}}
    results = run_central_search(request.target, request.algorithms)
    return {"results": results}