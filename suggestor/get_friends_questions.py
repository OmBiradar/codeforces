import requests
import hashlib
import time
import json
from requests.exceptions import ConnectionError, Timeout, RequestException
from dotenv import load_dotenv
import os

def generate_api_sig(api_key, secret_key, method_name, params):
    rand = "123456"  # Random 6-digit number
    current_time = str(int(time.time()))
    params_str = "&".join([f"{k}={v}" for k, v in sorted(params.items())])
    api_sig_str = f"{rand}/{method_name}?{params_str}#{secret_key}"
    api_sig_hash = hashlib.sha512(api_sig_str.encode('utf-8')).hexdigest()
    return rand + api_sig_hash, current_time

def get_user_friends(api_key, secret_key):
    method_name = "user.friends"
    params = {
        "apiKey": api_key,
        "time": str(int(time.time())),
        "onlyOnline": "false"
    }
    api_sig, current_time = generate_api_sig(api_key, secret_key, method_name, params)
    url = f"https://codeforces.com/api/{method_name}?onlyOnline=false&apiKey={api_key}&time={current_time}&apiSig={api_sig}"
    
    for attempt in range(5):  # Retry up to 5 times
        try:
            response = requests.get(url)
            if response.status_code == 200 and response.json()['status'] == 'OK':
                return response.json()['result']
            else:
                print("Error fetching friends")
                print(f"Status Code: {response.status_code}")
                print(f"Response Content: {response.content}")
                return []
        except (ConnectionError, Timeout) as e:
            print(f"Attempt {attempt + 1}: Connection error. Retrying...")
            time.sleep(2 ** attempt)  # Exponential backoff
        except RequestException as e:
            print(f"Attempt {attempt + 1}: Request error: {e}")
            return []
    return []

def get_user_submissions(handle):
    url = f"https://codeforces.com/api/user.status?handle={handle}"
    
    for attempt in range(5):  # Retry up to 5 times
        try:
            response = requests.get(url)
            if response.status_code == 200 and response.json()['status'] == 'OK':
                return response.json()['result']
            else:
                print(f"Error fetching submissions for {handle}")
                return []
        except (ConnectionError, Timeout) as e:
            print(f"Attempt {attempt + 1}: Connection error. Retrying...")
            time.sleep(2 ** attempt)  # Exponential backoff
        except RequestException as e:
            print(f"Attempt {attempt + 1}: Request error: {e}")
            return []
    return []

def get_unsolved_problems(user_handle, friends_handles):
    user_submissions = get_user_submissions(user_handle)
    user_solved_problems = {sub['problem']['name'] for sub in user_submissions if sub['verdict'] == 'OK'}

    unsolved_problems = {}
    for friend in friends_handles:
        time.sleep(2)  # Delay between API calls
        friend_submissions = get_user_submissions(friend)
        for sub in friend_submissions:
            problem_name = sub['problem']['name']
            if sub['verdict'] == 'OK' and problem_name not in user_solved_problems:
                if friend not in unsolved_problems:
                    unsolved_problems[friend] = []
                unsolved_problems[friend].append(sub['problem'])

    return unsolved_problems

def main():
    load_dotenv()
    user_handle = os.getenv('CODEFORCES_HANDLE')
    api_key = os.getenv('CODEFORCES_API_KEY')
    secret_key = os.getenv('CODEFORCES_SECRET_KEY')

    if not all([user_handle, api_key, secret_key]):
        raise ValueError("Please set CODEFORCES_HANDLE, CODEFORCES_API_KEY, and CODEFORCES_SECRET_KEY in .env file")
    
    friends_handles = get_user_friends(api_key, secret_key)
    unsolved_problems = get_unsolved_problems(user_handle, friends_handles)

    output = {}
    for friend, problems in sorted(unsolved_problems.items()):
        output[friend] = []
        for problem in problems:
            try:
                problem_info = {
                    "name": problem['name'],
                    "url": f"https://codeforces.com/problemset/problem/{problem['contestId']}/{problem['index']}",
                    "rating": problem.get('rating', None),
                    "tags": problem.get('tags', [])
                }
                output[friend].append(problem_info)
            except KeyError:
                print(f"Skipping problem {problem['name']} due to missing contestId or index")

    with open("unsolved_problems.json", "w") as f:
        json.dump(output, f, indent=4)

if __name__ == "__main__":
    main()