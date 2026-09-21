import pypdf
import sys

def extract(pdf_path, out_path):
    with open(pdf_path, 'rb') as f:
        reader = pypdf.PdfReader(f)
        text = ""
        for page in reader.pages:
            text += page.extract_text() + "\n"
    with open(out_path, 'w', encoding='utf-8') as out:
        out.write(text)

extract('C:/Users/lucas.drumond/.gemini/antigravity-ide/brain/9b44d75b-4e50-4ce4-8fdb-5d50a71c7ddf/.tempmediaStorage/media_1789760627826.pdf', 'phase5.txt')
