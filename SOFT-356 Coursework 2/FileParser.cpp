#include "FileParser.h"


// Public 
FileParser::FileParser() { }

bool FileParser::processObjFile(string filePath, Mesh*& mesh, string& materialFileName)
{

	ifstream ifile(filePath);

	vector< Vertex > verts;
	string materialName;
	int nameChangeCount = 0;
	vector < MaterialGroup > materialGroups;

	unsigned long preVertSize = 0;

	vector< vec4 > positions(1, vec4(0, 0, 0, 0));
	vector< vec3 > texcoords(1, vec3(0, 0, 0));
	vector< vec3 > normals(1, vec3(0, 0, 0));
	string lineStr;

	if (ifile >> lineStr) { // First check the file opens fine 

			while (getline(ifile, lineStr))
			{
				istringstream lineSS(lineStr);
				string lineType;
				lineSS >> lineType;

				// Get the name of the material file
				if (lineType == "mtllib")
				{
					regex self_regex("mtllib\\s(.*?).mtl");
					smatch unit;
					if (regex_search(lineStr, unit, self_regex)) {
						materialFileName = unit[1].str();
						materialFileName = materialFileName + ".mtl";
					}
				}

				// Vertex
				if (lineType == "v")
				{
					float x = 0, y = 0, z = 0, w = 1;
					lineSS >> x >> y >> z >> w;
					positions.push_back(vec4(x, y, z, w));
				}

				// Texture
				if (lineType == "vt")
				{
					float u = 0, v = 0, w = 0;
					lineSS >> u >> v >> w;
					texcoords.push_back(vec3(u, v, w));
				}

				// Normal
				if (lineType == "vn")
				{
					float i = 0, j = 0, k = 0;
					lineSS >> i >> j >> k;
					normals.push_back(normalize(vec3(i, j, k)));
				}

				// Polygon
				if (lineType == "f")
				{
					vector< VertRef > refs;
					string refStr;
					while (lineSS >> refStr)
					{
						istringstream ref(refStr);
						string vStr, vtStr, vnStr;
						getline(ref, vStr, '/');
						getline(ref, vtStr, '/');
						getline(ref, vnStr, '/');

						// Convert string to integer
						int v = atoi(vStr.c_str());
						int vt = atoi(vtStr.c_str());
						int vn = atoi(vnStr.c_str());

						// Check if the value is 0 or not
						v = (v >= 0 ? v : positions.size() + v);
						vt = (vt >= 0 ? vt : texcoords.size() + vt);
						vn = (vn >= 0 ? vn : normals.size() + vn);
						refs.push_back(VertRef(v, vt, vn));
					}

					for (size_t i = 1; i + 1 < refs.size(); ++i)
					{
						const VertRef* p[3] = { &refs[0], &refs[i], &refs[i + 1] };

						// Here we are calculating the surface normals
						// This is only needed if the normals arent present
						vec3 U(positions[p[1]->v] - positions[p[0]->v]); // edge 1
						vec3 V(positions[p[2]->v] - positions[p[0]->v]); // edge 2
						vec3 faceNormal = normalize(cross(U, V));

						for (size_t t = 0; t < 3; ++t)
						{
							Vertex vert;
							vert.position = vec3(positions[p[t]->v]);
							vert.texcoord = vec2(texcoords[p[t]->vt]);
							vert.normal = (p[t]->vn != 0 ? normals[p[t]->vn] : faceNormal); // <!-- Check if the normal is 0, if so then use the calculated value
							verts.push_back(vert);
						}
					}
				}

				if (lineType == "usemtl")
				{
					// if the name has already been set
					if (materialName.size() > 0) {

						if (verts.size() > 0) {

							// Loop through until we find the material name
							bool foundIt = false;
							for (int i = 0; i < materialGroups.size(); i++)
							{
								// If they are the same then add the index's to the array within
								if (materialGroups[i].materialName == materialName) {
									foundIt = true;
									pair <unsigned long, unsigned long> staEndIndex(preVertSize, (verts.size() - preVertSize));
									materialGroups[i].indexPairsArray.push_back(staEndIndex);

									// Set the previous vert size for reference
									preVertSize = verts.size();
								}

							}

							if (!foundIt)
							{
								MaterialGroup materialGroup;
								materialGroup.materialName = materialName;

								// We find the start index and the end
								pair <unsigned long, unsigned long> staLengthPair(preVertSize, (verts.size() - preVertSize));

								vector< pair <unsigned long, unsigned long> > indexes;
								indexes.push_back(staLengthPair);

								// Set the new index array
								materialGroup.indexPairsArray = indexes;

								// Add a new material group to the array
								materialGroups.push_back(materialGroup);

								// Set the previous vert size for reference
								preVertSize = verts.size();
							}
						}

						materialName = lineStr.substr(lineStr.find(" ") + 1);
						nameChangeCount += 1;

					}
					else {
						materialName = lineStr.substr(lineStr.find(" ") + 1);
						nameChangeCount += 1;
					}
				}

			}


		// We have got to the end of the file which means there might be something left to create
		if (verts.size() > 0) {

			// There was only one material in the file
			if (nameChangeCount <= 1) {
				// We add the first material group to the list
				MaterialGroup materialGroup;
				materialGroup.materialName = materialName;

				// We find the start index and the end
				pair <unsigned long, unsigned long> staLengthPair(0, verts.size());

				vector< pair <unsigned long, unsigned long> > indexes;
				indexes.push_back(staLengthPair);

				// Set the new index array
				materialGroup.indexPairsArray = indexes;

				// Add a new material group to the array
				materialGroups.push_back(materialGroup);

				// Set the previous vert size for reference
				preVertSize = verts.size();
			}

			if (verts.size() > preVertSize) {
				// Loop through until we find the material name
				bool foundIt = false;
				for (int i = 0; i < materialGroups.size(); i++)
				{
					// If they are the same then add the index's to the array within
					if (materialGroups[i].materialName == materialName) {
						foundIt = true;
						pair <unsigned long, unsigned long> staEndIndex(preVertSize, (verts.size() - preVertSize));
						materialGroups[i].indexPairsArray.push_back(staEndIndex);

					}
				}

				if (!foundIt)
				{
					MaterialGroup materialGroup;
					materialGroup.materialName = materialName;

					// We find the start index and the end
					pair <unsigned long, unsigned long> staLengthPair(preVertSize, (verts.size() - preVertSize));

					vector< pair <unsigned long, unsigned long> > indexes;
					indexes.push_back(staLengthPair);

					// Set the new index array
					materialGroup.indexPairsArray = indexes;

					// Add a new material group to the array
					materialGroups.push_back(materialGroup);

					// Set the previous vert size for reference
					preVertSize = verts.size();
				}
			}

			// Create mesh with everything we found
			mesh =
				new Mesh(
					verts.data(),
					verts.size(),
					nullptr,
					0,
					materialGroups.data(),
					materialGroups.size(),
					vec3(0.f, 0.f, 2.f),
					vec3(0.f),
					vec3(0, 0, 0),
					vec3(1.f)
				);

		}

		// Clear the verts array
		verts.clear();

		//Return true as it was completed
		return true;

	}
	else
	{
		// File didn't load correctly
		return false;
	}

}

bool FileParser::processMtlFile(string filePath, vector < Texture* >& textures, vector < Material* >& materials, int& textureCount)
{
	ifstream ifile(filePath);

	// We need some temporary variables
	string name;
	char fileName[255];

	vec3 ka{};
	vec3 kd{};
	vec3 ks{};
	float d = 1;
	float Ns = 0;
	GLuint mapKd = -1;
	GLuint mapKs = -1;
	GLuint mapD = -1;
	GLuint bumpMap = -1;

	float isDiffuseTex = 0;
	float isSpecularTex = 0;
	float isBumpTex = 0;

	bool materialInitStarted = false;

	string lineStr;

	if (ifile >> lineStr) { // First check the file opens fine 

		while (getline(ifile, lineStr))
		{
			istringstream lineSS(lineStr);
			string lineType;
			lineSS >> lineType;

			// Check for Ka - Ambient
			if (lineType == "Ka")
			{
				materialInitStarted = true;
				float x = 0, y = 0, z = 0;
				lineSS >> x >> y >> z;

				ka = (vec3(x, y, z));
			}

			// Check for Kd - Diffuse
			if (lineType == "Kd")
			{
				float x = 0, y = 0, z = 0;
				lineSS >> x >> y >> z;

				kd = (vec3(x, y, z));
			}

			// Check for Ks - Specular
			if (lineType == "Ks")
			{
				float x = 0, y = 0, z = 0;
				lineSS >> x >> y >> z;

				ks = (vec3(x, y, z));
			}

			// Check for opacity
			if (lineType == "d") {
				lineSS >> d;
			}

			// Check for Shininess (Ns)
			if (lineType == "Ns") {
				lineSS >> Ns;
			}

			// Check for map_Kd - Diffuse tex map
			if (lineType == "map_Kd")
			{
				string tempFileName = lineStr.substr(lineStr.find(" ") + 1);
				strcpy_s(fileName, tempFileName.c_str());
				textures.push_back(new Texture(fileName, GL_TEXTURE_2D));
				textureCount += 1;

				mapKd = textureCount;

				// A diffuse tex has been set
				isDiffuseTex = 1;
			}

			// Check for map_Ks - Specular tex map
			if (lineType == "map_Ks")
			{
				string tempFileName = lineStr.substr(lineStr.find(" ") + 1);
				strcpy_s(fileName, tempFileName.c_str());
				textures.push_back(new Texture(fileName, GL_TEXTURE_2D));
				textureCount += 1;

				mapKs = textureCount;

				// A Specular tex has been set
				isSpecularTex = 1;
			}

			// Check for map_d - Reflection map
			if (lineType == "map_d")
			{
				string tempFileName = lineStr.substr(lineStr.find(" ") + 1);
				strcpy_s(fileName, tempFileName.c_str());
				textures.push_back(new Texture(fileName, GL_TEXTURE_2D));
				textureCount += 1;

				mapKs = textureCount;
				// A specular tex has been set
				isDiffuseTex = 1;
			}

			// Check for map_Bump - Bump / Normal Mapping (Didn't get around to using this)
			if (lineType == "map_Bump")
			{
				string tempFileName = lineStr.substr(lineStr.find(" ") + 1);
				strcpy_s(fileName, tempFileName.c_str());
				textures.push_back(new Texture(fileName, GL_TEXTURE_2D));
				textureCount += 1;

				bumpMap = textureCount;
				// A bump tex has been set
				isBumpTex = 1;
			}

			// vertex
			if (lineType == "newmtl")
			{
				// We have a material to create
				if (materialInitStarted)
				{
					materials.push_back(new Material(ka, kd, ks, d, Ns, mapKd, mapKs, mapD, bumpMap, name, isDiffuseTex, isSpecularTex, isBumpTex));
				}

				name = lineStr.substr(lineStr.find(" ") + 1);
			}
		}

		// We are at the end of the file, do the materialInitStarted check again to see if we need to create a remaining material
		if (materialInitStarted)
		{
			materials.push_back(new Material(ka, kd, ks, d, Ns, mapKd, mapKs, mapD, bumpMap, name, isDiffuseTex, isSpecularTex, isBumpTex));
		}

		//Return true as it was completed
		return true;
	}
	else
	{
		// File didn't load correctly
		return false;
	}

}

bool FileParser::processColladaFile(string filePath, Mesh*& mesh, vector < Texture* >& textures, vector < Material* >& materials, int& textureCount)
{
	ifstream ifile(filePath);

	// Useful
	string previousTagName;

	/* ----- Material related variables ----- */
	string name;
	char fileName[255];
	vec3 ambient{ 1, 1, 1 };
	vec3 diffuse{ 1, 1, 1 };
	vec3 specular{ 1, 1, 1 };

	GLuint mapKd = -1;
	GLuint mapKs = -1;
	GLuint mapD = -1;
	GLuint bumpMap = -1;

	float isDiffuseTex = 0;
	float isSpecularTex = 0;
	float isBumpTex = 0;

	/* ----- Object related variables ----- */
	vector< Vertex > verts;
	vector< vec4 > positions(1, vec4(0, 0, 0, 0));
	vector< vec3 > texcoords(1, vec3(0, 0, 0));
	vector< vec3 > normals(1, vec3(0, 0, 0));

	// float array type (indicate whether position, normal or texcoords)
	string floatArrayType = "positions";
	string floatArrayValue = "";

	// input color causes offset
	bool colorOffset = false;

	string materialName;
	vector < MaterialGroup > materialGroups;

	// Reference to the previous vert size
	unsigned long preVertSize = 0;

	string lineStr;

	if (ifile >> lineStr) { // First check the file opens fine 

		while (getline(ifile, lineStr))
		{
			istringstream lineSS(lineStr);
			string lineType;
			lineSS >> lineType;

			/* ------------------------------ Firstly get all the material data ---------------------------- */

			// Get the name 
			if (lineStr.find("<effect id") != string::npos) {

				// Extract the material name from the line
				regex self_regex("<effect id=\"(\\w*)-effect\">");
				smatch unit;
				if (regex_search(lineStr, unit, self_regex)) {
					name = unit[1].str();
				}
			}

			// Set Ambience flag
			if (lineStr.find("<ambient") != string::npos) {
				previousTagName = "ambient";
			}

			// Set Diffuse flag
			if (lineStr.find("<diffuse") != string::npos) {
				previousTagName = "diffuse";
			}

			// Set Specular flag
			if (lineStr.find("<specular") != string::npos) {
				previousTagName = "specular";
			}

			// Get color tag
			if (lineStr.find("<color") != string::npos) {

				// Get the id and value with regex
				string id;
				string value;
				regex self_regex("<color sid=\\\"(\\w*)\\\">(.*?)<");
				smatch unit;

				if (regex_search(lineStr, unit, self_regex)) {
					id = unit[1].str();
					value = unit[2].str();
				}

				float x = 0, y = 0, z = 0;
				istringstream lineSS(value);
				string lineType;

				lineSS >> x >> y >> z;


				// Check which ID it is when setting the value
				if (id == "ambient")
				{
					ambient = vec3(x, y, z);
				}
				else if (id == "diffuse")
				{
					diffuse = vec3(x, y, z);
				}

			}

			// Get float
			if (lineStr.find("<texture") != string::npos) {

				// Get file name of texture 
				string tempFileName;
				regex self_regex("<texture texture=\"(.*?)-");
				smatch unit;

				if (regex_search(lineStr, unit, self_regex))
				{
					tempFileName = unit[1].str();
				}

				// Take this name and convert it into a file name ... (replace the '_' with '.')
				size_t n = tempFileName.find_last_of("_");
				tempFileName.replace(n, 1, ".");

				tempFileName = "textures/" + tempFileName;

				strcpy_s(fileName, tempFileName.c_str());

				// Check what previous tag was so we know what texture it is for
				if (previousTagName == "diffuse")
				{
					// Load the texture
					textures.push_back(new Texture(fileName, GL_TEXTURE_2D));
					textureCount += 1;
					mapKd = textureCount;

					// A diffuse tex has been set
					isDiffuseTex = 1;
				}
				else if (previousTagName == "specular") {

					// Load the texture
					textures.push_back(new Texture(fileName, GL_TEXTURE_2D));
					textureCount += 1;
					mapKs = textureCount;

					// A specular tex has been set
					isSpecularTex = 1;
				}

			}

			// Get float tag
			if (lineStr.find("<float sid") != string::npos) {

				// Get the id value with regex
				string id;
				regex self_regex("<float sid=\"(\w*)\">");
				smatch unit;

				if (regex_search(lineStr, unit, self_regex)) {
					id = unit[1].str();
				}

			}

			// We have reached the end of the material so construct it with what we have
			if (lineStr.find("</effect>") != string::npos) {
				// Construct with what we found
				materials.push_back(new Material(ambient, diffuse, specular, 1, 0, mapKd, mapKs, mapD, bumpMap, name, isDiffuseTex, isSpecularTex, isBumpTex));
			}



			/* --------------------- Then get the mesh details  ------------------------ */

			// Get the positions, coords and normals
			if (lineStr.find("<float_array id") != string::npos) {

				// Get the id value with regex and gets the type of float array
				string type;
				string value;
				regex self_regex("<float_array id=\".*?-(.*?)\"\\scount=\".*?\">(.*?)<\/float");
				smatch unit;

				if (regex_search(lineStr, unit, self_regex)) {
					type = unit[1].str(); // (Capture group one - Gets the id)
					value = unit[2].str(); // (Capture group two - Gets the value of float array)
				}

				floatArrayValue = value;

				if (type == "mesh-positions-array") {
					// Set positions flag
					floatArrayType = "positions";
				}
				else if (type == "mesh-normals-array") {
					// Set normals flag
					floatArrayType = "normals";
				}
				else if (type == "mesh-map-0-array" || "mesh-map-array") {
					// Set texcoords flag
					floatArrayType = "texcoords";
				}
			}

			// Get the stride 
			if (lineStr.find("<accessor source") != string::npos) {

				int stride = 0;

				regex self_regex("<accessor source=\".*?\"\\scount=\".*?\"\\sstride=\"(.*?)\">");
				smatch unit;
				if (regex_search(lineStr, unit, self_regex)) {
					stringstream temp(unit[1].str());
					temp >> stride;
				}

				// We go through the string and get x (stride) amount each time 
				istringstream ss(floatArrayValue);
				vector<tuple<string, string, string> > values;

				// then create either the positions, normals or texcoords
				if (floatArrayType == "positions") {

					positions.clear();

					// We go through the string and get x (stride) amount each time 
					istringstream ss(floatArrayValue);
					vector<tuple<string, string, string> > values;

					// count
					int count = 0;
					string prePreValue;
					string preValue;

					//Loop through values
					do {
						// Get the current value
						string value;
						ss >> value;

						switch (count) {
						case 0:
							prePreValue = value;
							break;
						case 1:
							preValue = value;
							break;
						case 2:
							values.push_back(make_tuple(prePreValue, preValue, value));
							count = -1;
							break;
						}

						count += 1;
					} while (ss);

					// Loop through the values and get x amount then create a position
					for (size_t i = 0; i < values.size(); i++)
					{
						float x = stof(get<0>(values[i]));
						float y = stof(get<1>(values[i]));
						float z = stof(get<2>(values[i]));
						float w = 1;
						positions.push_back(
							vec4(x, y, z, w)
						);
					}

				}
				else if (floatArrayType == "normals") {

					normals.clear();

					// We go through the string and get x (stride) amount each time 
					istringstream ss(floatArrayValue);
					vector<tuple<string, string, string> > normValues;

					// count
					int count = 0;
					string prePreValue;
					string preValue;

					//Loop through values
					do {

						// Get the current value
						string value;
						ss >> value;

						switch (count) {
						case 0:
							prePreValue = value;
							break;
						case 1:
							preValue = value;
							break;
						case 2:
							normValues.push_back(make_tuple(prePreValue, preValue, value));
							count = -1;
							break;
						}

						count += 1;

					} while (ss);

					// Loop through the values and get x amount then create a position
					for (size_t i = 0; i < normValues.size(); i++)
					{
						float x = stof(get<0>(normValues[i]));
						float y = stof(get<1>(normValues[i]));
						float z = stof(get<2>(normValues[i]));
						normals.push_back(vec3(x, y, z));

					}
				}
				else if (floatArrayType == "texcoords") {

					texcoords.clear();

					// We go through the string and get x (stride) amount each time 
					istringstream ss(floatArrayValue);
					vector<tuple<string, string> > texValues;

					// count
					int count = 0;
					string preValue;

					//Loop through values
					do {

						// Get the current value
						string value;
						ss >> value;

						if (value != "")
						{
							switch (count) {
							case 0:
								preValue = value;
								break;
							case 1:
								texValues.push_back(make_tuple(preValue, value));
								count = -1;
								break;
							}

							count += 1;
						}

					} while (ss);

					// Loop through the values and get x amount then create a position
					for (size_t i = 0; i < texValues.size(); i++)
					{
						float x = stof(get<0>(texValues[i]));
						float y = stof(get<1>(texValues[i]));
						float z = 0;
						texcoords.push_back(vec3(x, y, z));
					}
				}
			}


			// Get the material name
			if (lineStr.find("<triangles material") || lineStr.find("<polylist material") != string::npos) {

				if (lineStr.find("<triangles material") != string::npos)
				{
					regex self_regex("<triangles material=\"(.*?)-material\"\\scount=\".*?\">");
					smatch unit;
					if (regex_search(lineStr, unit, self_regex)) {
						materialName = unit[1].str();
					}
				}

				if (lineStr.find("<polylist material") != string::npos)
				{
					regex self_regex("<polylist material=\"(.*?)-material\"\\scount=\".*?\">");
					smatch unit;
					if (regex_search(lineStr, unit, self_regex)) {
						materialName = unit[1].str();
					}
				}

			}

			// Check for this as it may be a offset of 4 instead of 3
			if (lineStr.find("<input semantic=\"COLOR") != string::npos) {
				colorOffset = true;
			}

			// Get the indices / Faces and workout the vertices
			if (lineStr.find("<p>") != string::npos) {

				string pValues;
				regex self_regex("<p>(.*?)</p");
				smatch unit;
				if (regex_search(lineStr, unit, self_regex)) {
					pValues = unit[1].str();
				}

				// We go through the string and get x (stride) amount each time 
				istringstream ss(pValues);
				vector<tuple<int, int, int >> vecPValues;
				//vector<VertRef> refs;

				// count
				int count = 0;
				int prePreValue;
				int preValue;
				int preTestValue;

				if (!colorOffset) {
					//Loop through values
					do {
						// Get the current value
						string value;
						ss >> value;

						if (value != "") {
							switch (count) {
							case 0:
								prePreValue = stoi(value);
								break;
							case 1:
								preValue = stoi(value);
								break;
							case 2:
								vecPValues.push_back(make_tuple(prePreValue, preValue, stoi(value)));
								count = -1;
								break;
							}
							count += 1;
						}

						else {

						}

					} while (ss);

				}
				else {
					//Loop through values
					do {
						// Get the current value
						string value;
						ss >> value;

						if (value != "") {
							switch (count) {
							case 0:
								prePreValue = stoi(value);
								break;
							case 1:
								preValue = stoi(value);
								break;
							case 2:
								// Do nothing
								preTestValue = stoi(value);
								break;
							case 3:
								vecPValues.push_back(make_tuple(prePreValue, preValue, preTestValue));
								count = -1;
								break;
							}
							count += 1;
						}

						else {

						}

					} while (ss);

					// Set it back to false after this
					colorOffset = false;
				}



				// Now create the verticies with everything we have collected for this mesh
				for (size_t i = 0; i < vecPValues.size(); i++)
				{
					Vertex vert;
					vert.position = vec3(positions[(get<0>(vecPValues[i]))]);
					vert.normal = normals[(get<1>(vecPValues[i]))];
					vert.texcoord = vec2(texcoords[(get<2>(vecPValues[i]))]);
					verts.push_back(vert);
				}

				// Now create the material group
				if (verts.size() > 0) {
					// Loop through until we find the material name
					bool foundIt = false;
					for (int i = 0; i < materialGroups.size(); i++)
					{
						// If they are the same then add the index's to the array within
						if (materialGroups[i].materialName == materialName) {
							foundIt = true;
							pair <unsigned long, unsigned long> staEndIndex(preVertSize, (verts.size() - preVertSize));
							materialGroups[i].indexPairsArray.push_back(staEndIndex);

							// Set the previous vert size for reference
							preVertSize = verts.size();
						}

					}

					if (!foundIt)
					{
						MaterialGroup materialGroup;
						materialGroup.materialName = materialName;

						// We find the start index and the end
						pair <unsigned long, unsigned long> staLengthPair(preVertSize, (verts.size() - preVertSize));

						vector< pair <unsigned long, unsigned long> > indexes;
						indexes.push_back(staLengthPair);

						// Set the new index array
						materialGroup.indexPairsArray = indexes;

						// Add a new material group to the array
						materialGroups.push_back(materialGroup);

						// Set the previous vert size for reference
						preVertSize = verts.size();
					}
				}

			}

		}


		// Finally we can create the mesh
		mesh =
			new Mesh(
				verts.data(),
				verts.size(),
				nullptr,
				0,
				materialGroups.data(),
				materialGroups.size(),
				vec3(0.f, 0.f, 2.f),
				vec3(0.f),
				vec3(0, 0, 0),
				vec3(7.f)
			);


		//Return true as it was completed
		return true;
	}
	else
	{
		// File didn't load correctly
		return false;
	}



}

void FileParser::createObject(Mesh*& mesh, vector < Texture* >& textures, vector < Material* >& materials, vector < Model* >& models)
{
	/*
	// If there is more than one model then space them out along x axis
	if (models.size() > 0) {
		models.push_back(new Model(
			vec3(models.size() * 10, 0.f, 2.f),
			materials,
			mesh
		));
	}
	else
	{
		models.push_back(new Model(
			// Do this so it puts the model in the middle of the terrain to start off 
			vec3(20.f, 0.f, 20.f),
			materials,
			mesh
		));
	}
	*/

	models.push_back(new Model(
		// Do this so it puts the model in the middle of the terrain to start off 
		vec3(20.f, 0.f, 20.f),
		materials,
		mesh
	));
}
