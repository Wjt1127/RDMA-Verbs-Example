#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <rdma/rdma_cma.h>
#include <infiniband/verbs.h>

int
main(void)
{
	int			num_devices, count, n;
	struct ibv_context	**list, **lptr;
	struct ibv_device_attr	dev_attrs;
	char			*tptr, *nptr;


	num_devices = 0;
	list = rdma_get_devices(&num_devices);
	if(list == NULL || num_devices == 0) {
		fprintf(stderr, "No RDMA devices found\n");
		exit(EXIT_FAILURE);
	}

	fprintf(stderr, "%d RDMA devices found\n", num_devices);

	for(count = 1, lptr = list; *lptr != NULL; lptr++, count++) {
		switch ((*lptr)->device->transport_type) {
		case IBV_TRANSPORT_IB:
			tptr = "InfiniBand";
			break;
		case IBV_TRANSPORT_IWARP:
			tptr = "iWARP";
			break;
		default:
			tptr = "Unknown transport type";
			break;
		}	/* switch */

		switch ((*lptr)->device->node_type) {
		case IBV_NODE_CA:
			nptr = "CA";
			break;
		case IBV_NODE_SWITCH:
			nptr = "Switch";
			break;
		case IBV_NODE_ROUTER:
			nptr = "Router";
			break;
		case IBV_NODE_RNIC:
			nptr = "RNIC";
			break;
		default:
			nptr = "Unknown node type";
			break;
		}	/* switch */
		fprintf(stderr, "%d: %s, %s, %s, %s\n",
			count, (*lptr)->device->name,
			(*lptr)->device->dev_name, tptr, nptr);

		if((n = ibv_query_device(*lptr, &dev_attrs)) != 0)
		{
			if(n < 0)
				n = -n;
			fprintf(stderr, "ibv_query_device returned error %d: "
				"%s\n", n, strerror(n));
		continue;
		}
		fprintf(stderr, "\tRDMA device vendor id %u, part id %u, "
			"hardware version %u, firmware version %s\n",
			dev_attrs.vendor_id,
			dev_attrs.vendor_part_id, dev_attrs.hw_ver,
			dev_attrs.fw_ver);
		fprintf(stderr, "\tMaximum number of supported queue pairs - "
			"max_qp %d\n", dev_attrs.max_qp);
		fprintf(stderr, "\tMaximum number of outstanding work requests "
			"- max_qp_wr %d\n", dev_attrs.max_qp_wr);
		fprintf(stderr, "\tMaximum number of scatter-gather items per "
			"work request for non-registered queue pairs"
			"- max_sge %d\n", dev_attrs.max_sge);
		fprintf(stderr, "\tMaximum number of scatter-gather items per "
			"work request for registered queue pairs"
			"- max_sge_rd %d\n", dev_attrs.max_sge_rd);
		fprintf(stderr, "\tMaximum number of supported completion "
			"queues - max_cq %d\n", dev_attrs.max_cq);
		fprintf(stderr, "\tMaximum number of elements per completion "
			"queue - max_cqe %d\n", dev_attrs.max_cqe);
		fprintf(stderr, "\tMaximum number of supported memory "
			"regions - max_mr %d\n", dev_attrs.max_mr);
		fprintf(stderr, "\tMaximum number of supported protection "
			"domains - max_pd %d\n", dev_attrs.max_pd);
	}

	rdma_free_devices(list);

	return 0;
}	/* main */
