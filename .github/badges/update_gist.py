import json
import requests
import typing


def update_gist(packet: typing.Dict,
        file_name: str,
        uri: str,
        token: str):

    """ Update a file in a given gist

    Arguments
    ---------
    packet: typing.Dict
        the new contents to upload to the gist file
    file_name: str
        the name of the gist file to update
    uri: str
        the uniform resource identifier (URI) of the gist
    token: str
        the authorization token for the gist
    """

    body = {
        'files': {
            file_name: {
                'content': json.dumps(packet)
            }
        }
    }

    headers = {'Authorization': f'token {token}'}
    url = f'https://api.github.com/gists/{uri}'
    request = requests.patch(url, json=body, headers=headers)

    print(f'packet: {packet}')
    print(f'response: {request.status_code}')
