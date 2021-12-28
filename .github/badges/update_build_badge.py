import argparse
import os
import sys
import typing

from update_gist import update_gist


def _create_packet(passing: bool) -> typing.Dict:
    """ Create a build badge packet

    Arguments
    ---------
    passing: bool
        true if the build was successful

    Returns
    -------
    packet: typing.Dict
        the badge packet -- including label, message, and colour
    """

    # Create packet
    packet = {
        'cacheSeconds': 0,
        'label': 'build',
        'message': ('passing' if passing else 'failing'),
        'color': ('brightgreen' if passing else 'red'),
        'schemaVersion': 1
    }

    return packet


if __name__ == '__main__':

    # Parse arguments
    parser = argparse.ArgumentParser()
    parser.add_argument('return_code', type=int)
    parser.add_argument('uri', type=str)
    parser.add_argument('file_name', type=str)
    arguments = parser.parse_args()

    # Create a badge packet from the coverage percentage
    packet = _create_packet(arguments.return_code == 0)

    # Update a gist file with the packet, authorizing with a token
    token = os.environ.get('GIST_TOKEN')
    update_gist(packet, arguments.file_name, arguments.uri, token)
