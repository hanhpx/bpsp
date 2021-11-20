#include <assert.h>
#include <stdio.h>

#include "client.h"
#include "topic.h"

void find_sub(bpsp__topic_tree* tree, char* topic) {
    UT_array* subs = topic__tree_find_subscribers(tree, topic, 0);
    printf("FINDING TOPIC: %s\n", topic);

    bpsp__subscriber* p;
    for (p = (bpsp__subscriber*)utarray_front(subs); p != NULL; p = (bpsp__subscriber*)utarray_next(subs, p)) {
        printf("%s\n", p->_id);
    }

    printf("\n");

    utarray_free(subs);
}

int main() {
    bpsp__client* client = client__new(NULL, NULL);
    bpsp__topic_tree tree = {};
    topic__init_tree(&tree);
    bpsp__topic_tree* ptree = topic__new_tree();
    bpsp__subscriber* sub = subscriber__new("locationA/sensorA/*", client, NULL);

    status__err s;
    /* uint16_t n_tok = 0; */
    /* char* first_tok = NULL; */
    /* s = topic__extract_token("./bar", &n_tok, &first_tok); */
    /* if (first_tok) { */
    /*     free(first_tok); */
    /* } */
    /* s = topic__extract_token("foo/.*", &n_tok, &first_tok); */
    /* free(first_tok); */
    /* s = topic__extract_token("foo/+/bar", &n_tok, &first_tok); */
    /* free(first_tok); */

    s = topic__add_subscriber(&tree, sub);

    bpsp__client* client1 = client__new(NULL, NULL);
    bpsp__subscriber* sub1 = subscriber__new("locationA/sensorA/uptime", client1, NULL);
    s = topic__add_subscriber(&tree, sub1);
    bpsp__client* client2 = client__new(NULL, NULL);
    bpsp__subscriber* sub2 = subscriber__new("locationA/sensorA/uptime", client2, NULL);
    s = topic__add_subscriber(&tree, sub2);
    /* subscriber__free(sub1); */

    bpsp__subscriber* sub3 = subscriber__new("locationA/sensorA/downtime", client, NULL);
    s = topic__add_subscriber(&tree, sub3);

    bpsp__subscriber* sub4 = subscriber__new("locationA/sensorA/uptime", client, NULL);
    s = topic__add_subscriber(&tree, sub4);
    topic__print_tree(&tree);
    s = topic__del_subscriber(&tree, sub4, 1);
    topic__print_tree(&tree);
    s = topic__del_subscriber(&tree, sub, 1);
    topic__print_tree(&tree);
    s = topic__del_subscriber(&tree, sub, 1);
    topic__print_tree(&tree);
    s = topic__del_subscriber(&tree, sub2, 1);
    topic__print_tree(&tree);
    s = topic__del_subscriber(&tree, sub3, 1);
    topic__print_tree(&tree);
    s = topic__del_subscriber(&tree, sub1, 1);
    topic__print_tree(&tree);
    s = topic__del_subscriber(&tree, sub3, 1);
    topic__print_tree(&tree);
    subscriber__free(sub4);
    subscriber__free(sub1);
    subscriber__free(sub2);
    subscriber__free(sub3);
    subscriber__free(sub);

    sub = subscriber__new("locationA/sensorA/+", client, NULL);
    s = topic__add_subscriber(&tree, sub);
    subscriber__free(sub);

    sub = subscriber__new("locationB/sensorB", client, NULL);
    s = topic__add_subscriber(&tree, sub);
    subscriber__free(sub);

    sub = subscriber__new("locationB/*/", client, NULL);
    s = topic__add_subscriber(&tree, sub);
    subscriber__free(sub);

    sub = subscriber__new("locationB/*/alo", client, NULL);
    s = topic__add_subscriber(&tree, sub);
    subscriber__free(sub);

    sub = subscriber__new("locationB/+/uptime", client, NULL);
    s = topic__add_subscriber(&tree, sub);
    subscriber__free(sub);

    sub = subscriber__new("locationA/+/uptime", client, NULL);
    s = topic__add_subscriber(&tree, sub);
    subscriber__free(sub);

    sub = subscriber__new("*+/locationA/+/uptime", client, NULL);
    s = topic__add_subscriber(&tree, sub);
    subscriber__free(sub);

    /* topic__print_tree(ptree); */

    topic__print_tree(&tree);

    ASSERT_ARG(s == BPSP_OK, 1);

    find_sub(&tree, "locationA/sensorA");
    find_sub(&tree, "locationA/sensorA/uptime");
    find_sub(&tree, "locationA/sensorB/uptime");
    find_sub(&tree, "locationB/sensorB");
    find_sub(&tree, "locationB/sensorA");

    topic__dtor_tree(&tree);
    topic__free_tree(ptree);

    client__free(client);
    client__free(client1);
    client__free(client2);

    return 0;
}
