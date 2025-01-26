import json

def sort_problems_by_rating(file_path):
    with open(file_path, 'r') as f:
        data = json.load(f)

    for friend, problems in data.items():
        data[friend] = sorted(problems, key=lambda x: x['rating'] if x['rating'] is not None else float('inf'))

    with open(file_path, 'w') as f:
        json.dump(data, f, indent=4)

    return data

sorted_problems = sort_problems_by_rating('unsolved_problems.json')

for friend, problems in sorted_problems.items():
    print(f"Problems solved by {friend} that you should start solving:")
    for problem in problems:
        print(f"{problem['name']} - {problem['url']} (Rating: {problem['rating']})")