import { GenericForm } from "./GenericForm";

export function EditAp(props) {
    let custom_field_mapping = {
			"authMode": {
				html_type: "select",
				json_type: "string", 
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
        type="ap"
        apiHandler={props.apiHandler}
        custom_field_mapping={custom_field_mapping}
        />
}

export function EditStation(props) {
    return <GenericForm
        type="station"
        apiHandler={props.apiHandler}
        />
}

export function EditOta(props) {
    let custom_field_mapping = {
        "romUrl": {
            html_type: "url",
            json_type: "string"
        },
        "spiffUrl": {
            html_type: "url",
            json_type: "string"
        }
    }
    return <GenericForm
        type="ota"
        apiHandler={props.apiHandler}
        custom_field_mapping={custom_field_mapping}
        />
}