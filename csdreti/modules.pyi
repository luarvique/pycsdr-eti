from pycsdr.modules import Module, Writer

version: str = ...
csdreti_version: str = ...


class EtiDecoder(Module):
    def __init__(self):
        ...

    def setMetaWriter(self, writer: Writer) -> None:
        ...