import random
import math
import numpy as np
from collections import defaultdict
import csv

'''
You can use the following libraries: numpy, pandas, and any Python standard libraries.
If you want to use any other third-party library, please contact the TA.

<<< Don't forget to input the arguments (k, T, N) of the optimization function (main.py) !! >>>
'''


def distance_keys(key1, key2, keyboard):
    """
   Calculates the Euclidean distance between two keys on a given keyboard.

   Parameters:
   - key1 (str): The first key.
   - key2 (str): The second key.
   - keyboard (list): A 2D list representing the keyboard layout.

   Returns:
   - float: The distance between the two keys in millimeters.
   """
    # Initialize coordinates for both keys
    x1, y1 = 0, 0
    x2, y2 = 0, 0
    # Iterate through rows and columns of the keyboard layout
    for i in range(5):
        for j in range(6):
            # Find the coordinates of key1
            if keyboard[i][j] == key1:
                y1 = -i + 5  # Adjust the y-coordinate for the inverted layout
                x1 = j + 1
            # Find the coordinates of key2
            elif keyboard[i][j] == key2:
                y2 = -i + 5  # Adjust the y-coordinate for the inverted layout
                x2 = j + 1

    # Calculate and return the Euclidean distance between the two keys in millimeters
    distance = math.sqrt((x2 - x1) ** 2 + (y2 - y1) ** 2) * 1000
    return distance


def fitts_law(D, W, a, b):
    """
    Computes the movement time using Fitts' Law formula.

    Parameters:
    - D (float): The distance between the start and target points.
    - W (float): The width of the target.
    - a (float): The intercept parameter in Fitts' Law.
    - b (float): The slope parameter in Fitts' Law.

    Returns:
    - float: The calculated movement time.
    """

    # Calculate the movement time using Fitts' Law formula
    movement_time = a + b * np.log2(D / W + 1)
    return movement_time


def calculate_average_movement_time(layout, transitional_frequencies, a, b):
    """
    Calculate the average movement time for a given keyboard layout.

    Parameters:
    - layout (list of lists): The 5x6 keyboard layout.
    - transitional_frequencies (dict): Dictionary containing transitional frequencies between keys.
    - a (float): The intercept parameter in Fitts' Law.
    - b (float): The slope parameter in Fitts' Law.

    Returns:
    - float: The calculated average movement time.
    """

    total_time = 0  # Accumulator for total movement time
    total_pairs = 0  # Counter for total pairs of keys

    # Iterate through each key in the layout
    for i in range(len(layout)):
        for j in range(len(layout[0])):
            key1 = str(layout[i][j]).lower()  # Convert to lowercase

            if key1 == '*':
                continue  # Skip pairs with "*"

            # Iterate through every other key in the layout
            for m in range(len(layout)):
                for n in range(len(layout[0])):
                    if i != m or j != n:
                        key2 = str(layout[m][n]).lower()  # Convert to lowercase

                        if key2 == '*':
                            continue  # Skip pairs with "*"

                        # Check both possible orders of keys in the transitional_frequencies dictionary
                        frequency = transitional_frequencies.get((key1, key2), 0.1)
                        D = distance_keys(key1, key2, layout)  # Use your distance_keys function to calculate distance
                        W = 1  # Assuming key width is 1 for simplicity

                        # Calculate movement time using Fitts' Law
                        movement_time = fitts_law(D, W, a, b) * frequency

                        # Accumulate total time and count total pairs
                        total_time += movement_time
                        total_pairs += 1

    # Calculate the average movement time
    average_time = total_time / total_pairs
    return average_time


def calculate_transitional_frequencies(word_frequencies):
    """
    Calculate transitional frequencies between consecutive letters in words.

    Parameters:
    - word_frequencies (dict): Dictionary containing word frequencies.

    Returns:
    - dict: Dictionary containing transitional frequencies between letter pairs.
    """

    # Initialize dictionaries
    letter_frequencies = defaultdict(float)

    # Define all possible lowercase letter pairs
    all_letter_pairs = [(chr(97 + i), chr(97 + j)) for i in range(26) for j in range(26)]

    # Initialize transitional frequencies dictionary with zero values
    transitional_frequencies = {pair: 0 for pair in all_letter_pairs}

    # Iterate through words and update letter frequencies and transitional frequencies
    for word, frequency_per_million in word_frequencies.items():
        for i in range(len(word)):
            if i < len(word) - 1 and word[i].isalpha() and word[i + 1].isalpha():
                letter1, letter2 = word[i].lower(), word[i + 1].lower()

                if letter1 != letter2:
                    letter_frequencies[letter1] += frequency_per_million
                    transitional_frequencies[(letter1, letter2)] += frequency_per_million

    # Calculate transitional frequencies
    for letter_pair, count in transitional_frequencies.items():
        first_letter = letter_pair[0]
        transitional_frequency = count / letter_frequencies[first_letter]
        transitional_frequencies[letter_pair] = transitional_frequency

    return transitional_frequencies


def read_word_frequencies_from_csv(csv_file):
    """
    Read word frequencies from a CSV file and return a dictionary.

    Parameters:
    - csv_file (str): Path to the CSV file containing word frequencies.

    Returns:
    - dict: Dictionary containing word frequencies.
    """

    word_frequencies = {}

    # Open the CSV file for reading
    with open(csv_file, 'r') as file:
        # Create a CSV reader
        reader = csv.reader(file)

        # Skip the header row
        next(reader)

        # Iterate through rows in the CSV
        for index, row in enumerate(reader):
            # Get values using indices
            frequency_column_value = row[1]
            word_column_value = row[2]

            # Continue with your processing
            frequency = float(frequency_column_value)
            word = ''.join(word_column_value)

            # Check if the lowercase version of the word exists in the dictionary
            lowercase_word = word.lower()

            if lowercase_word in word_frequencies:
                # If it exists, add the frequencies
                word_frequencies[lowercase_word] += frequency
            else:
                # Otherwise, create a new entry
                word_frequencies[lowercase_word] = frequency

    return word_frequencies


def metropolis_algorithm(layout, transitional_frequencies, a, b, k, T, num_iterations):
    """
    Runs the Metropolis algorithm to optimize the keyboard layout.

    Parameters:
    - layout (list): Current keyboard layout.
    - transitional_frequencies (dict): Transitional frequencies between keys.
    - a (float): Touchscreen keyboarding - Fitts' law parameter.
    - b (float): Touchscreen keyboarding - Fitts' law parameter.
    - k (float): Coefficient for the Metropolis algorithm.
    - T (float): Temperature for the Metropolis algorithm.
    - num_iterations (int): Number of iterations for the Metropolis algorithm.

    Returns:
    - final_keyboard_layout (list): Optimized keyboard layout result.
    - t_value_sequence (list): Changes of the Fitts’ law movement time value recorded during the optimization process.

    Note:
    - The function uses the Metropolis algorithm to optimize the keyboard layout by swapping keys and
      accepting/rejecting the new layout based on energy changes and probabilities.
    """

    # Calculate the initial average movement time
    current_energy = calculate_average_movement_time(layout, transitional_frequencies, a, b)

    # Flatten the initial layout
    current_layout = np.array(layout).flatten().tolist()

    # Initialize the list to record changes in Fitts' law movement time values
    t_value_sequence = [round(current_energy, 6)]

    for _ in range(num_iterations):
        # Randomly select two keys and swap their positions
        index1, index2 = random.sample(range(len(current_layout)), 2)

        new_layout = current_layout.copy()
        new_layout[index1], new_layout[index2] = new_layout[index2], new_layout[index1]

        # Calculate the new average movement time
        new_energy = calculate_average_movement_time(np.array(new_layout).reshape(5, 6).tolist(),
                                                     transitional_frequencies, a, b)

        # Calculate energy change and decide whether to accept the new layout
        energy_change = new_energy - current_energy
        probability = math.exp(-energy_change / (k * T))

        if energy_change < 0 or random.random() < probability:
            current_layout = new_layout
            current_energy = new_energy

        t_value_sequence.append(round(current_energy, 6))  # Round the new value to 6 decimal places

    # Reshape the final layout to a 5x6 matrix
    final_keyboard_layout = np.array(current_layout).reshape(5, 6).tolist()

    return final_keyboard_layout, t_value_sequence


def optimization(a, b, k, T, N, keyboard_layout):
    """
    Parameter
    1. a = 0.083 ( Touchscreen keyboarding - Fitts’ law parameter )
    2. b = 0.127 ( Touchscreen keyboarding - Fitts’ law parameter )
    3. k : k is a coefficient, input your value in main.py
    4. T : T is temperature, input your value in main.py
    5. N : Number of iteration, input your value in main.py
    6. keyboard_layout : Initial keyboard layout ( Python list )

    Return
    1. final_keyboard_layout : Optimized keyboard layout result (Python list format)
    2. t_value_sequence : Changes of the Fitts’ law movement time value recorded during the optimization process
        (Python list format)

    Result example
    1. Optimized keyboard layout result
        (1) '*' represents an empty key.
        (2) The keyboard layout list below is identical to Figure 1.

    Optimized_keyboard_layout_result_list = [
        ['Q', 'W', 'E', 'R', 'T', 'Y'],
        ['U', 'I', 'O', 'P', 'A', 'S'],
        ['D', 'F', 'G', 'H', 'J', 'K'],
        ['L', 'Z', 'X', 'C', 'V', 'B'],
        ['N', 'M', '*', '*', '*', '*']]

    2. Changes of the Fitts’ law movement time value recorded during the optimization process
        (1) The first element represents the Fitts’ Law value (t) of the initial keyboard layout.
        (2) The numbers are just examples.

    Changes_in_t = [3.312849, 2.980117, ..., 1.851442]

    Note
    1. You can use the "pandas" or "csv" library to read a CSV file.
    2. When reading CSV files, always use "relative path".
    3. Do not change the function name and structure.
    4. Your function should not read input from the user and print the results of your function.

    < Code explanation >

    The function performs the optimization of a touchscreen keyboard layout using the Metropolis algorithm.
    It reads word frequencies from a CSV file, calculates transitional frequencies, and then applies
    the Metropolis algorithm to optimize the given keyboard layout. The resulting optimized layout and
    the changes in Fitts’ law movement time are returned as a tuple.
    """
    # Define the path to the word frequency CSV file
    csv_file_path = 'dataset/word_frequency.csv'

    # Read word frequencies from the CSV file
    word_frequencies = read_word_frequencies_from_csv(csv_file_path)

    # Calculate transitional frequencies based on word frequencies
    transitional_frequencies = calculate_transitional_frequencies(word_frequencies)

    # Run the Metropolis algorithm to optimize the keyboard layout
    final_keyboard_layout, t_value_sequence = metropolis_algorithm(
        keyboard_layout, transitional_frequencies, a, b, k, T, N
    )

    # Return the optimized layout and the sequence of Fitts’ law movement time changes
    return final_keyboard_layout, t_value_sequence
