from multiprocessing import Pool

def mapper(line):
    words = line.strip().split()

    key_value_pairs = [(word, 1) for word in words]
    return key_value_pairs

def reducer(key_value_pairs):
    word_counts = {}
    for key, value in key_value_pairs:
        if key in word_counts:
            word_counts[key] += value
        else:
            word_counts[key] = value

    return list(word_counts.items())

def map_reduce(input_file):
    with open(input_file, 'r') as file:
        lines = file.readlines()

    with Pool() as pool:
        intermediate_key_value_pairs = pool.map(mapper, lines)

    intermediate_key_value_pairs = [pair for sublist in intermediate_key_value_pairs for pair in sublist]

    grouped_key_value_pairs = {}
    for key, value in intermediate_key_value_pairs:
        if key in grouped_key_value_pairs:
            grouped_key_value_pairs[key].append((key, value))
        else:
            grouped_key_value_pairs[key] = [(key, value)]

    with Pool() as pool:
        final_key_value_pairs = pool.map(reducer, grouped_key_value_pairs.values())

    final_key_value_pairs = [pair for sublist in final_key_value_pairs for pair in sublist]

    for key, value in final_key_value_pairs:
        print(f'{key}: {value}')

input_file = 'cheet.txt'  
map_reduce(input_file)