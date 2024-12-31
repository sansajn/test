// Octree implementaiton from *Real-time collision detection* book Chapter 7.3 Trees.

struct Point {
	float x, y, z;
};

Point operator+(Point const & a, Point const & b) {
	return {a.x + b.x, a.y + b.y, a.z + b.z};
}

/* TODO: Based on the code from InsertObject sutracting two 
points leads into distance of that points and not another point. 
Look the book how operation should be implemented. */
// float oprator-(Point const & a, Point const & b) {}

struct Object {
	Point center;  //!< Center point for object
	float radius;  //!< Radius of object bounding sphere
	Object * pNextObject;  //!< Pointer to next object when linked into list
};

struct Node {
	Point center;
	float halfWidth;
	Node * pChild[8];
	Object * pObjList;  //!< Represents usefull data asociated with an octree node.
};

Node * BuildOctree(Point center, float halfwidth, int stopDepth) {
	if (stopDepth < 0) return nullptr;
	else {
		Point offset;
		Node * pNode =  new Node;
		pNode->center = center;
		float step = halfwidth * 0.5f;
		for (int i = 0; i < 8; ++i) {
			offset.x = ((i & 1) ? step : -step);
			offset.y = ((i & 2) ? step : -step);
			offset.z = ((i & 4) ? step : -step);
			pNode->pChild[i] = BuildOctree(center + offset, step, stopDepth - 1);
		}
		return pNode;
	}
}

void InsertObject(Node * pTree, Object * pObject) {
	int index = 0, straddle = 0;
	// Compute the octant number [0..7] the object sphere center is in
	// If straddling any of the dividing x, y, or z planes, exit directly
	for (int i = 0; i < 3; ++i) {
		float delta = pObject->center[i] - pTree->center[i];
		if (Abs(delta) < pTree->halfWidth + pObject->radius) {
			straddle = 1;
			break;
		}
		if (delta > 0.0f) index |= (1 << i);  // ZYX
	}

	if (!straddle && pTree->pChild[index]) {
		// Fully contained in existing child node; insert in that subtree
		InsertObject(pTree->pChild[index], pObject);
	} else {
		// Straddling, or no child node to descend into, so
		// link object into linked list at this node
		pObject->pNextObject = pTree->pObjList;
		pTree->pObjList = pObject;
	}
}

int main(int argc, char * argv[]) {
	// building octree with depth=3 Q: Is it empty?
	Point center{0,0,0};
	Node * root = BuildOctree(center, 100.0f, 3);


	return 0;
}
