import os
import requests
import typing
import json

def create_coverage_content(coverage: int):

    packet = {
        'cacheSeconds': 0,
        'label': 'coverage',
        'message': f'{coverage}%',
        'schemaVersion': 1
    }

    # Decide what colour the badge should be, based on percentage
    colours = [
        (65, 'orange'),
        (75, 'yellow'),
        (85, 'yellowgreen'),
        (90, 'green'),
        (95, 'brightgreen')
    ]
    colour = 'red'
    for pair in colours:
        if pair[0] > coverage:
            break
        colour = pair[1]
    packet['color'] = colour

    return packet


def create_build_content(passing: bool):
    return {
        'cacheSeconds': 0,
        'label': 'build',
        'color': 'brightgreen' if passing else 'red',
        'message': 'passing' if passing else 'failing',
        'schemaVersion': 1
    }


def update_gist(content: typing.Dict,
        file_name: str,
        token: str,
        uri: str):

    packet = {
        'files': {
            file_name: {
                'content': json.dumps(content)
            }
        }
    }

    headers = {'Authorization': f'token {token}'}
    url = f'https://api.github.com/gists/{uri}'
    data = json.dumps(packet)
    request = requests.patch(url, json=packet, headers=headers)

    print(request.json())


if __name__ == '__main__':
    gist_secret = os.environ.get('GIST_SECRET')
    gist_id = os.environ.get('GIST_ID')

    content = create_coverage_content(95)
    file_name = 'pimlico-coverage-badge.json'
    update_gist(content, file_name, gist_secret, gist_id)
