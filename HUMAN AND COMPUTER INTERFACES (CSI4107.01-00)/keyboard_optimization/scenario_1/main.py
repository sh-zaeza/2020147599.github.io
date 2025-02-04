from optimization import optimization
import pickle

if __name__ == '__main__':
    '''
    This is the main function for automatic grading.
    Do not change the function name and structure.

    << Don't forget to input the arguments (k, T, N) of the optimization function!! >>
    '''

    # Input your StudentID
    YourStudentID = 2020147599

    initial_keyboard_layout = [
        ['Q', 'W', 'E', 'R', 'T', 'Y'],
        ['U', 'I', 'O', 'P', 'A', 'S'],
        ['D', 'F', 'G', 'H', 'J', 'K'],
        ['L', 'Z', 'X', 'C', 'V', 'B'],
        ['N', 'M', '*', '*', '*', '*']]

    k = 0.1
    T = 1.0
    N = 1000

    keyboard_layout, t_value_sequence = optimization(a=0.083, b=0.127, k=k, T=T, N=N,
                                                     keyboard_layout=initial_keyboard_layout)

    # Save - Keyboard layout
    with open(f"{YourStudentID}_keyboard.pkl", 'wb') as f:
        pickle.dump(keyboard_layout, f, pickle.HIGHEST_PROTOCOL)

    # Save - t value sequence
    with open(f"{YourStudentID}_time.pkl", 'wb') as f:
        pickle.dump(t_value_sequence, f, pickle.HIGHEST_PROTOCOL)

    print(keyboard_layout, t_value_sequence[0], t_value_sequence[-1])