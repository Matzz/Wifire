import { GenericForm } from "./GenericForm";

export default function EditAp(props) {
    let customFieldMapping = {
			authMode: {
				htmlType: "select",
				jsonType: "string", 
				values: {
					0: "Open",
					1: "WEP",
					2: "WEP PSK",
					3: "WPA PSK",
					4: "WPA2 PSK",
					5: "WPA WPA2 PSK",
					6: "WPA2 ENTERPRISE",
					7: "MAX"
				}
			}
		}
    return <GenericForm
        title="Edit access point configuration"
        type="ap"
        apiHandler={props.apiHandler}
        customFieldMapping={customFieldMapping}
        />
}