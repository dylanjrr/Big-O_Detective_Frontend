# Big-O Detective Frontend

A C++ command-line application for analyzing algorithm time complexity by fitting runtime data to various Big-O complexity models.

## Features

- **Load Dataset**: Import runtime data from a text file (format: input_size runtime per line)
- **Manual Entry**: Add data points manually through the console
- **View Dataset**: Display loaded data in a formatted table
- **Run Analysis**: Evaluate data against multiple complexity models:
  - O(1) - Constant
  - O(log n) - Logarithmic
  - O(n) - Linear
  - O(n log n) - Linearithmic
  - O(n²) - Quadratic
  - O(n³) - Cubic
- **Save Report**: Generate a detailed report with analysis results
- **Backend Integration**: Optional connection to Python Flask backend for distributed analysis

## Building

### Requirements
- C++17 or later
- C++17 compatible compiler (g++, clang++, MSVC)
- For backend integration: WinINet (Windows) or libcurl (Linux/Mac)

### Compilation

**Without backend integration (local analysis only):**
```bash
g++ -std=c++17 -o big_o_detective src/*.cpp
```

Or with MSVC:
```cmd
cl /EHsc /std:c++17 src/*.cpp
```

**With backend integration (Windows - uses WinINet):**
```cmd
cl /EHsc /std:c++17 src/*.cpp wininet.lib
```

**With backend integration (Linux/Mac - uses libcurl):**
```bash
g++ -std=c++17 -o big_o_detective src/*.cpp -lcurl
```

## Usage

Run the executable:
```bash
./big_o_detective
```

The application will ask if you want to use the backend server:
- **Yes (y)**: Connects to the Python Flask backend for analysis
- **No (n)**: Uses local C++ analysis engine

Follow the on-screen menu to:
1. Load a dataset file (e.g., `data/sample.txt`)
2. Enter data points manually (input size and runtime)
3. View the current dataset
4. Run complexity analysis
5. Save the report to `reports/report.txt`

## Backend Integration

The frontend can optionally connect to a Python Flask backend server for distributed analysis:

### Setup Backend

1. Navigate to the backend directory:
```bash
cd ../Big-O_Detective_Backend
```

2. Install dependencies:
```bash
pip install -r requirements.txt
```

3. Start the backend server:
```bash
python app.py
```

The backend will start on `http://localhost:5000`

### Benefits of Backend Integration

- **Centralized Analysis**: Multiple clients can share the same analysis engine
- **Language Flexibility**: Backend uses Python with NumPy for efficient numerical computations
- **Scalability**: Easy to add database persistence and multi-user support
- **API Access**: REST API can be used by other applications

### API Endpoints

The backend provides the following endpoints:
- `POST /api/dataset` - Create a new dataset
- `GET /api/dataset/{id}` - Retrieve a dataset
- `DELETE /api/dataset/{id}` - Delete a dataset
- `POST /api/analyze` - Run complexity analysis
- `POST /api/report` - Generate a report
- `GET /api/health` - Health check

## Data Format

Input files should contain one data point per line:
```
<input_size> <runtime>
```

Example:
```
100 0.5
200 1.2
400 2.8
800 6.1
1600 13.5
```

## Project Structure

```
Big-O_Detective_Frontend/
├── include/           # Header files
│   ├── BackendClient.h      # HTTP client for backend communication
│   ├── ComplexityModel.h
│   ├── Dataset.h
│   ├── FittingEngine.h
│   ├── ReportGenerator.h
│   └── UI.h
├── src/              # Source files
│   ├── BackendClient.cpp    # HTTP client implementation
│   ├── ComplexityModel.cpp
│   ├── Dataset.cpp
│   ├── FittingEngine.cpp
│   ├── ReportGenerator.cpp
│   ├── UI.cpp
│   └── main.cpp
├── data/             # Sample data files
├── reports/          # Generated reports (auto-created)
└── tests/            # Test files
```

## How It Works

The application uses R² (coefficient of determination) to measure how well each complexity model fits the runtime data:
- Transforms input sizes according to each complexity model
- Performs linear regression to compute R² score
- Selects the model with the highest R² as the best fit
- Displays a visual chart comparing all models

When using the backend:
- Data is sent to the backend via HTTP POST requests
- Backend performs the same R² analysis using Python/NumPy
- Results are returned as JSON and displayed in the frontend

## Error Handling

- Validates input sizes (must be positive)
- Validates runtimes (cannot be negative)
- Checks if analysis has been run before saving reports
- Creates output directories automatically if they don't exist
- Provides descriptive error messages for file operations
- Gracefully falls back to local analysis if backend connection fails
