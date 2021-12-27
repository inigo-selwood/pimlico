import argparse
import os
import typing

from update_gist import update_gist


def _create_packet(coverage: float) -> typing.Dict:
    """ Create a coverage badge packet

    Uses the coverage value to create a badge -- varying the colour by
    percentage

    Arguments
    ---------
    coverage: float
        the code coverage percentage, expressed as a floating-point number

    Returns
    -------
    packet: typing.Dict
        the badge packet -- including label, message, and colour
    """

    # Create packet
    packet = {
        'cacheSeconds': 0,
        'label': 'coverage',
        'message': f'{int(coverage)}%',
        'schemaVersion': 1
    }

    colours = [
        (65, 'orange'),
        (75, 'yellow'),
        (85, 'yellowgreen'),
        (90, 'green'),
        (95, 'brightgreen')
    ]

    # Decide what colour the badge should be, based on percentage
    colour = 'red'
    for pair in colours:
        if pair[0] > coverage:
            break
        colour = pair[1]
    packet['color'] = colour

    return packet


if __name__ == '__main__':

    # Parse arguments
    parser = argparse.ArgumentParser()
    parser.add_argument('coverage', type=float)
    parser.add_argument('uri', type=str)
    parser.add_argument('file_name', type=str)
    arguments = parser.parse_args()

    # Create a badge packet from the coverage percentage
    packet = _create_packet(arguments.coverage)

    # Update a gist file with the packet, authorizing with a token
    token = os.environ.get('GIST_TOKEN')
    update_gist(packet, arguments.file_name, arguments.uri, token)
