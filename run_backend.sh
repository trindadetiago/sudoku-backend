#!/bin/bash

# Check if the virtual environment exists and activate it
if [ -f venv/bin/activate ]; then
    source venv/bin/activate
    echo "Virtual environment activated."
else
    echo "Virtual environment not found, continuing without activation."
fi

# Start the FastAPI server
echo "Starting the FastAPI backend..."
uvicorn backend.main:app --reload --host 0.0.0.0 --port 8000
