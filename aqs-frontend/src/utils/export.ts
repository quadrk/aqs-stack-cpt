import { Measurement } from "../services/api";
import { saveAs } from "file-saver";
import Papa from "papaparse";
import * as XLSX from "xlsx";
import jsPDF from "jspdf";
import autoTable from "jspdf-autotable";

export function exportAsCSV(data: Measurement[]) {
  const csv = Papa.unparse(data);
  const bom = "\uFEFF";
  const blob = new Blob([csv], { type: "text/csv;charset=utf-8;" });
  saveAs(blob, `measurements_${new Date().toISOString()}.csv`);
}

export function exportAsXLSX(data: Measurement[]) {
  const worksheet = XLSX.utils.json_to_sheet(data);
  const workbook = XLSX.utils.book_new();
  XLSX.utils.book_append_sheet(workbook, worksheet, "Measurements");

  const buffer = XLSX.write(workbook, { bookType: "xlsx", type: "array" });
  const blob = new Blob([buffer], { type: "application/octet-stream" });
  saveAs(blob, `measurements_${new Date().toISOString()}.xlsx`);
}

export function exportAsPDF(data: Measurement[]) {
  const doc = new jsPDF();
  const tableData = data.map((m) => [
    m.device_id,
    m.sensor_id,
    new Date(m.ts).toLocaleString(),
    m.value.toFixed(2),
  ]);
  doc.setFont("helvetica", "normal");


  autoTable(doc, {
    head: [["device", "sensor", "timestamp", "value"]],
    body: tableData,
  });

  doc.save(`measurements_${new Date().toISOString()}.pdf`);
}
