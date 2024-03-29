import { GenericForm } from "./GenericForm";

export default function EditOta(props) {
    let customFieldMapping = {
        romUrl: {
            htmlType: "url",
            jsonType: "string"
        },
        spiffUrl: {
            htmlType: "url",
            jsonType: "string"
        }
    }
    return <GenericForm
        title="Edit OTA configuration"
        type="ota"
        apiHandler={props.apiHandler}
        customFieldMapping={customFieldMapping}
        submitButtons={[
            {text: "Save and update", actionName: "saveAndUpdate"},
            {text: "Save only", actionName: "saveOnly"}
        ]}
        />
}