#!/usr/bin/env python3

"""
  Extract the binary tag from the firmware binary:
    python3 blynk-tag.py extract ./raw_firmware.bin ./tag.bin

  Create a new binary tag manually:
    python3 blynk-tag.py create ./tag.bin --mcu 0.1.2 --type TMPL0123456 --build "Apr 19 2023 12:01:07" --blynk "0.3.0"

  Show info in a tag or a firmware:
    python3 blynk-tag.py show ./tag.bin
    python3 blynk-tag.py show ./raw_firmware.bin

"""
import re

def create_tag(taginfo):
    taginfo = map(lambda x: x.encode('utf-8'), taginfo)
    return b'\0'.join(taginfo) + b'\0\0'

def find_tag(data):
    r = re.compile(b'blnkinf\\x00[\\w\\s\\.,:\\-\\(\\)\\x00]*?\\x00\\x00')
    match = r.search(data)
    if match is not None:
        return match[0]
    return None

def parse_tag(tag):
    def pairwise(t):
        it = iter(t)
        return zip(it,it)

    taginfo = tag.split(b'\0')
    taginfo = list(map(lambda x: x.decode('utf-8'), taginfo))
    return list(pairwise(taginfo[1:-2]))

if __name__ == "__main__":
    import sys
    import argparse

    def run_extract(args):
        with open(args.file_in, 'rb') as f:
            data = f.read()
            tag = find_tag(data)
        if tag is None:
            print("Blynk info tag not found", file=sys.stderr)
            sys.exit(1)
        with open(args.file_out, 'wb') as f:
            f.write(tag)

    def run_show(args):
        with open(args.file_in, 'rb') as f:
            data = f.read()
            tag = find_tag(data)
        if tag is None:
            print("Blynk info tag not found", file=sys.stderr)
            sys.exit(1)

        for n,v in parse_tag(tag):
            print(f"{n}: {v}")

    def run_create(args):
        taginfo = ["blnkinf"]
        taginfo.extend(["mcu",      args.mcu])
        taginfo.extend(["fw-type",  args.type])
        if args.build:
            taginfo.extend(["build", args.build])
        if args.blynk:
            taginfo.extend(["blynk", args.blynk])

        tag = create_tag(taginfo)
        with open(args.file_out, 'wb') as f:
            f.write(tag)

    parser = argparse.ArgumentParser(description='Blynk firmware tag utility')
    subparsers = parser.add_subparsers()
    create  = subparsers.add_parser('create')
    create.add_argument('--mcu',     help='Version of the MCU firmware', required=True)
    create.add_argument('--type',    help='Firmware type (usually same as TemplateID)', required=True)
    create.add_argument('--build',   help='Firmware build date and time')
    create.add_argument('--blynk',   help='Blynk library version')
    create.add_argument('file_out', metavar='FILE_OUT',  help='output file')
    create.set_defaults(func=run_create)

    extract = subparsers.add_parser('extract')
    extract.add_argument('file_in',  metavar='FILE_IN',   help='input file')
    extract.add_argument('file_out', metavar='FILE_OUT',  help='output file')
    extract.set_defaults(func=run_extract)

    show = subparsers.add_parser('show')
    show.add_argument('file_in',  metavar='FILE_IN',   help='input file')
    show.set_defaults(func=run_show)

    args = parser.parse_args()
    if "func" in args:
        args.func(args)
    else:
        parser.print_usage()
