import random
import math
import numpy as np
from collections import defaultdict

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


def calculate_average_movement_time(layout, transitional_frequencies, a, b, fat_finger_error_prob):
    """
    Calculates the average movement time for a given keyboard layout.

    Parameters:
    - layout (list of lists): The 5x6 keyboard layout.
    - transitional_frequencies (dict): Dictionary containing transitional frequencies between keys.
    - a (float): The intercept parameter in Fitts' Law.
    - b (float): The slope parameter in Fitts' Law.
    - fat_finger_error_prob (float): Probability of a fat-finger error.

    Returns:
    - float: The calculated average movement time.
    """

    total_time = 0
    total_pairs = 0

    # Iterate through each key in the layout
    for i in range(len(layout)):
        for j in range(len(layout[0])):
            key1 = str(layout[i][j]).lower()  # Convert to lowercase

            # Iterate through every other key in the layout
            for m in range(len(layout)):
                for n in range(len(layout[0])):
                    key2 = str(layout[m][n]).lower()  # Convert to lowercase

                    # Skip pairs with a backspace
                    if key1 == 'backspace' or key2 == 'backspace':
                        continue

                    frequency = transitional_frequencies.get((key1, key2), 0.1)
                    D = distance_keys(key1, key2, layout)  # Use your distance_keys function to calculate distance
                    W = 1  # Assuming key width is 1 for simplicity

                    # Apply fat finger error probability
                    if random.random() < fat_finger_error_prob and key1 != key2:
                        frequency += fat_finger_error_prob

                    movement_time = fitts_law(D, W, a, b) * frequency

                    total_time += movement_time
                    total_pairs += 1

    # Calculate the average movement time
    average_time = total_time / total_pairs
    return average_time


def calculate_transitional_frequencies(fat_finger_error_prob):
    """
    Calculates transitional frequencies between consecutive keys in the writing text.

    Parameters:
    - fat_finger_error_prob (float): Probability of a fat-finger error.

    Returns:
    - dict: Dictionary containing transitional frequencies between keys.
    """

    file_path = 'dataset/writing.txt'

    # Initialize dictionaries
    letter_frequencies = defaultdict(float)

    # Initialize transitional frequencies dictionary
    transitional_frequencies = {}

    # Read the content of writing.txt
    with open(file_path, 'r') as file:
        text = file.read().replace('\n', ' ')

    # Iterate through the text
    for i in range(len(text) - 1):
        key1 = text[i].lower()  # Convert to lowercase
        letter_frequencies[key1] += 1

        key2 = text[i + 1].lower()  # Convert the next character to lowercase

        # Exclude pairs of duplicate keys
        if key1 != key2:
            # Apply fat finger error probability
            if random.random() < fat_finger_error_prob:
                transitional_frequencies[('backspace', key2)] = transitional_frequencies.get(('backspace', key2), 0) + 1
                transitional_frequencies[(key1, 'backspace')] = transitional_frequencies.get((key1, 'backspace'), 0) + 1
            else:
                transitional_frequencies[(key1, key2)] = transitional_frequencies.get((key1, key2), 0) + 1

    return transitional_frequencies


def metropolis_algorithm(layout, transitional_frequencies, a, b, k, T, num_iterations, fat_finger_error_prob):
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
    - fat_finger_error_prob (float): Probability of fat finger errors.

    Returns:
    - final_keyboard_layout (list): Optimized keyboard layout result.
    - t_value_sequence (list): Changes of the Fitts’ law movement time value recorded during the optimization process.

    Note:
    - The function uses the Metropolis algorithm to optimize the keyboard layout by swapping keys and
      accepting/rejecting the new layout based on energy changes and probabilities.
    """

    # Flatten the initial layout
    current_layout = np.array(layout).flatten().tolist()

    # Calculate the initial average movement time
    current_energy = calculate_average_movement_time(
        np.array(current_layout).reshape(5, 6).tolist(), transitional_frequencies, a, b, fat_finger_error_prob
    )

    # Initialize the list to record changes in Fitts' law movement time values
    t_value_sequence = [round(current_energy, 6)]

    for _ in range(num_iterations):
        # Randomly select two keys and swap their positions
        index1, index2 = random.sample(range(len(current_layout)), 2)

        new_layout = current_layout.copy()
        new_layout[index1], new_layout[index2] = new_layout[index2], new_layout[index1]

        # Calculate the new average movement time
        new_energy = calculate_average_movement_time(
            np.array(new_layout).reshape(5, 6).tolist(), transitional_frequencies, a, b, fat_finger_error_prob
        )

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
        (1) The keyboard layout list below is identical to Figure 2.

    Optimized_keyboard_layout_result_list = [
        ['Q', 'W', 'E', 'R', 'T', 'Y'],
        ['U', 'I', 'O', 'P', 'A', 'S'],
        ['D', 'F', 'G', 'H', 'J', 'K'],
        ['L', 'Z', 'X', 'C', 'V', 'B'],
        ['N', 'M', 'Space', 'Backspace', '-', ',']]

    2. Changes of the Fitts’ law movement time value recorded during the optimization process
        (1) The first element represents the Fitts’ Law value (t) of the initial keyboard layout.
        (2) The numbers are just examples.

    Changes_in_t = [3.312849, 2.980117, ..., 1.851442]

    Note
    1. When reading .txt files, always use "relative path".
    2. Do not change the function name and structure.
    3. Your function should not read input from the user and print the results of your function.


    < Code explanation >

    - The primary goal is to optimize a keyboard layout based on Fitts' Law, which models the time it takes to move
      between keys as a function of the distance and size of the keys.
    - Fitts' Law is used to calculate the movement time between keys. It takes into account the distance between keys
      (D), the width of the target key (W), and parameters (a and b).
    - The Metropolis algorithm is employed for optimization. It involves starting with an initial keyboard layout and
      iteratively making small changes to the layout. These changes are accepted or rejected based on energy
      differences and probabilities.
    - The program starts with an initial keyboard layout (a 5x6 matrix representing keys).
    - Transitional frequencies between consecutive keys are calculated based on the writing text. These frequencies
      represent the likelihood of moving from one key to another.
    - The Metropolis algorithm iterates a specified number of times (N). In each iteration, two keys are randomly
      selected and their positions are swapped to create a new layout. The energy change (difference in Fitts'
      Law movement time) between the current and new layout is calculated. The new layout is accepted with a certain
      probability determined by the Metropolis criterion. If the energy change is negative or a random number is less
      than the acceptance probability, the new layout is adopted; otherwise, it is rejected. The process continues for
      the specified number of iterations.
    - The final optimized keyboard layout and a sequence of Fitts' Law movement time values are obtained. The final
      layout and the sequence of movement time values are printed as output.
    - A fat-finger error probability is considered, meaning that occasionally a user might press a key adjacent to the
      intended key.
    - The final optimized keyboard layout and the sequence of movement time values are printed for analysis. This
    logic combines concepts from Fitts' Law and the Metropolis algorithm to iteratively improve a keyboard layout,
    considering real-world typing behaviors and transitional frequencies in the provided writing text. The result is
    a layout that minimizes the average movement time between keys.

    """

    # Set the fat finger error probability
    fat_finger_error_prob = 0.04

    # Calculate transitional frequencies from writing.txt
    transitional_frequencies = calculate_transitional_frequencies(fat_finger_error_prob)

    # Run the Metropolis algorithm to optimize the keyboard layout
    final_keyboard_layout, t_value_sequence = metropolis_algorithm(
        keyboard_layout, transitional_frequencies, a, b, k, T, N, fat_finger_error_prob
    )

    return final_keyboard_layout, t_value_sequence
