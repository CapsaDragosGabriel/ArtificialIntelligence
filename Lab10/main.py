import rdflib
from rdflib import Graph
import random as r

lista_mare = []


def get_q1(number):
    q1 = "Relatia dintre {} si {}?"
    my_triple = lista_mare[number]
    # iau termenu din stanga
    c1 = my_triple[0]
    curr_concepts = []
    # vad in ce relatii participa conceptul respectiv(stanga sau dreapta)
    for triplet in lista_mare:
        if triplet[0] == c1:
            curr_concepts.append((triplet[2], triplet[1]))
        elif triplet[2] == c1:
            curr_concepts.append((triplet[0], triplet[1]))

    # aleg un alt concept random
    c2 = r.randint(0, len(curr_concepts) - 1)
    # formatez intrebarea
    q1 = q1.format(c1, curr_concepts[c2][0])
    # setez raspunsul
    relation = curr_concepts[c2][1]
    # returnez intrebarea + raspunsul(relatia in care se afla c1 cu c2)
    return q1, relation


def get_q2(number):
    q2 = "Cine e in relatie de {} cu {}?"
    my_triple = lista_mare[number]
    c1 = my_triple[0]

    # vad in ce relatii participa conceptul respectiv (stanga sau dreapta)
    # si adaug toate conceptele cu care are legatura intr-o lista
    curr_concepts = []
    for triplet in lista_mare:
        if triplet[0] == c1:
            curr_concepts.append((triplet[1], triplet[2]))
        elif triplet[2] == c1:
            curr_concepts.append((triplet[1], triplet[0]))

    # iau o relatie random
    random_relation = r.randint(0, len(curr_concepts) - 1)
    c2 = []
    # daca am relatia respectiva adaug in lista
    for relation, concept in curr_concepts:
        # daca are loc relatia c1 randomRelation c2 adaug c2 la lista
        if curr_concepts[random_relation][0] == relation:
            c2.append(concept)

    q2 = q2.format(curr_concepts[random_relation][0], c1)
    # returnez intrebarea + raspunsul(o lista de concepte care se afla in relatia number cu conceptul c1
    return q2, c2


def generate_questions():
    while True:
        # aleg un triplet
        triple_random = r.randint(0, len(lista_mare) - 1)

        # dau cu banul sa vad ce fel de intrebare primesc
        number = r.randint(0, 1)

        if number == 0:
            question, response = get_q1(triple_random)
            print(question)
            print(response)
            input_response = input()
            if response == input_response:
                print("Corect!")
            elif input_response == 'Exit':
                break
            else:
                print("Gresit!")
        else:
            question, response_list = get_q2(triple_random)
            print(question)
            print(response_list)
            input_response = input()
            if input_response in response_list:
                print("Corect!")
            elif input_response == 'Exit':
                break
            else:
                print("Gresit!")


def print_all(g):
    for s, p, o in g:
        # skip la ce-i blank
        if type(s) is not rdflib.BNode and \
                type(p) is not rdflib.BNode and \
                type(o) is not rdflib.term.Literal and\
                type(o) is not rdflib.BNode:
            if type(s) is rdflib.URIRef:
                s = g.qname(s)
            if type(p) is rdflib.URIRef:
                p = g.qname(p)
            if type(o) is rdflib.URIRef:
                o = g.qname(o)
            print((s, p, o))
            lista_mare.append((s, p, o))


if __name__ == "__main__":
    g = Graph()
    g.parse("food.rdf")
    print_all(g)
    print("\n\n\n\nQuiz time:\n")
    generate_questions()



