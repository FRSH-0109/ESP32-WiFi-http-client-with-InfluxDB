token = "t0UCqk_xS4-6wJv2hhTA6pD5PHP9TCL4_de7sqQwHdxxAidjKJ2j9ABpbmbzOpU2bgNP4jdRjnazMns5AvyLww=="

from influxdb_client import InfluxDBClient

# InfluxDB connection details
url = "https://eu-central-1-1.aws.cloud2.influxdata.com"  # Your cloud URL
org = "SCIR_Project"
bucket = "ESP32_Reciever"

# Initialize the client
client = InfluxDBClient(url=url, token=token, org=org)

# Define the Flux query to retrieve recent data
query = f'''
    from(bucket: "{bucket}")
    |> range(start: -1m)  // Adjust time range as needed
    |> filter(fn: (r) => r["_measurement"] == "SCIR")  // Match Telegraf's measurement name
    |> filter(fn: (r) => r["_field"] == "BME280_Pressure")  // Match Telegraf's field's name
'''

# Execute the query
query_api = client.query_api()
result = query_api.query(query)

# Process and print the results
for table in result:
    for record in table.records:
        print(f"Time: {record.get_time()}, Field: {record.get_field()}, Value: {record.get_value()}")

# Close the client
client.close()